/*
    The script language is very simple. It consists of a
    movement and then an action.

    <token1> <token2>

    Every line in the script file must have no more then 2 white
    space separated tokens followed by a newline.

    The possible commands for <token1> are:

        last    - go to the last line
        first   - go to the first line
        delete  - delete line n
        goto    - move to line n eg. goto 25
        edit    - edit a file

    The possible commands for <token2> are

        file    - for use with edit
        n       - for use with goto, move & delete

    -------------------------------------------------------

    Note: write() will block if the slave device input
    queue is full which is probably fine because vim
    will start using it up and the write will resume after
    but if vim is ever stuck on some input it could block
    forever. Maybe make it nonblocking and have a max block
    time.

    Note: If there is ever a swap file on a file you are
    trying to edit vim will block with a warning message
    for a fraction of a second which can cause any new
    commands written to the master fd to be discarded.

    Todo: Figure out a way to process backend errors. This
    will require parsing the interactive output of vim after
    each command for error lines.

    [drivi program (parent)]    [vim in ex mode]
        |               |              |
        |               |              |
    [terminal]    [pty master]----[pty slave]
        |
        |
    [user]
*/

#include "tlpi_hdr.h" // IWYU pragma: export

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/wait.h>

#include "pty_fork.h"
#include "dynamic_array.h"

#define BUF_SIZE 256

int main(int argc, char **argv) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s drivi-script-file", argv[0]);

    struct termios prev_termios;
    if (tcgetattr(STDIN_FILENO, &prev_termios) == -1)
        errExit("tcgetattr");

    struct winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
        errExit("ioctl-TIOCGWINSZ");

    int master_fd;

    int child_pid = ptyFork(&master_fd, NULL, 0, &prev_termios, &ws);
    if (child_pid == -1)
        errExit("ptyFork");

    if (child_pid == 0) {
        execlp("ex", "ex", (char *) NULL);
        errExit("execlp");
    }

    int script_fd = open(argv[1], O_RDONLY, 0600);
    if (script_fd == -1)
        errExit("open");

    /* Read script file into memory */

    DynArray lines;
    if (da_init(&lines, 0, 256) == -1)
        errExit("da_init");

    char c[2] = "\0";
    int num_read;

    while (1) {
        if ((num_read = read(script_fd, c, 1)) == -1)
            errExit("read");
        if (num_read == 0)
            break;
        if (c[0] == '\n')
            continue;

        char *line = da_push(&lines);
        *line = '\0';

        strcat(line, c);

        while (1) {
            if ((num_read = read(script_fd, c, 1)) == -1)
                errExit("read");
            if (c[0] == '\n' || num_read == 0)
                break;
            strcat(line, c);
        }
    }

    printf("Processing script file:\n");
    for (size_t i = 0; i < lines.size; ++i )
        printf("\t%s\n", (char *) da_get(&lines, i));
    printf("\n");

    /* Process each command and send the equivalent to vim */

    char cmd[BUF_SIZE];
    cmd[0] = '\0';

    for (size_t i = 0; i < lines.size; ++i) {
        /* Process tokens */
        char *token1 = strtok((char *) da_get(&lines, i), " ");
        printf("token1: %s   ", token1);
        if (!token1) {
            fprintf(stderr, "drivi: syntax: token1\n");
            exit(EXIT_FAILURE);
        }

        char *token2 = strtok(NULL, " ");
        printf("token2: %s   ", token2);
        if (!token2) {
            fprintf(stderr, "drivi: syntax: token2\n");
            exit(EXIT_FAILURE);
        }

        if (strtok(NULL, "") != NULL) {
            fprintf(stderr, "drivi: syntax: 3 or more tokens\n");
            exit(EXIT_FAILURE);
        }

        /* Interpret tokens */

        if (strcmp(token1, "last") == 0) {
            if (write(master_fd, "$", 1) != 1)
                errExit("Partial/Failed write");
        } else if (strcmp(token1, "first") == 0) {
            if (write(master_fd, "1", 1) != 1)
                errExit("Partial/Failed write");
        } else if (strcmp(token1, "delete") == 0) {
            getInt(token2, GN_NONNEG, "n");   /* Make sure its a number */
            strcat(cmd, token2);
            strcat(cmd, "d");
            strcat(cmd, "\n");

            if (write(master_fd, cmd, strlen(cmd)) != (int) strlen(cmd))
                errExit("Partial/Failed write");
        } else if (strcmp(token1, "goto") == 0) {
            getInt(token2, GN_NONNEG, "n");   /* Make sure its a number */
            strcat(cmd, token2);
            strcat(cmd, "\n");

            if (write(master_fd, cmd, strlen(cmd)) != (int) strlen(cmd))
                errExit("Partial/Failed write");
        } else if (strcmp(token1, "edit") == 0) {
            if (open(token2, O_RDONLY, 0600) == -1) {
                fprintf(stderr, "drivi: edit: File doesn't exist\n");
                exit(EXIT_FAILURE);
            }

            strcat(cmd, "e ");
            strcat(cmd, token2);
            strcat(cmd, "\n");

            if (write(master_fd, cmd, strlen(cmd)) != (int) strlen(cmd))
                errExit("Partial/Failed write");
        } else {
            fprintf(stderr, "drivi: syntax: unrecognized command\n");
            exit(EXIT_FAILURE);
        }

        printf("Sent to vim: %s\n", cmd);
        cmd[0] = '\0';
    }

    da_free(&lines);

    /* Save and quit vim */

    if (write(master_fd, "w\n", 2) != 2)
        errExit("Partial/Failed write");
    if (write(master_fd, "q!\n", 3) != 3)
        errExit("Partial/Failed write");

    /* Wait for slave to die before exiting so we do
     * not send it SIGHUP and interupt any commands */

    int epfd = epoll_create1(0);
    if (epfd == -1)
        errExit("epoll_create1");

    struct epoll_event ev;
    ev.events = EPOLLHUP;
    ev.data.fd = master_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, master_fd, &ev) == -1)
        errExit("epoll_ctl");

    printf("\nAbout to call epoll_wait()\n");

    if (epoll_wait(epfd, &ev, 1, -1) == -1)
        errExit("epoll_wait");

    if (ev.events & EPOLLHUP)
        printf("epoll reports EPOLLHUP\n");
}
