#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/epoll.h>

#include "pty_fork.h"
#include "tty_functions.h"

#define BUF_SIZE 256

struct termios prev_termios;

void reset_tty(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &prev_termios) == -1)
        errMsg("tcsetattr");
}

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s longrunner arg...", argv[0]);

    if (tcgetattr(STDIN_FILENO, &prev_termios) == -1)
        errExit("tcgetattr");
    if (atexit(reset_tty) == -1)
        errExit("atexit");

    ttySetRaw(STDIN_FILENO, NULL);

    struct winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
        errExit("ioctl");

    int master_fd;
    int childpid = ptyFork(&master_fd, NULL, 0, &prev_termios, &ws);
    if (childpid == -1)
        errExit("ptyFork");

    if (childpid == 0) {
        /* add multi arg support */
        execlp(argv[1], argv[1], (char *) NULL);
        errExit("execlp");
    }

    int epfd = epoll_create1(0);
    if (epfd == -1)
        errExit("epoll_create1");

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1)
        errExit("epoll_ctl");

    ev.events = EPOLLIN;
    ev.data.fd = master_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, master_fd, &ev) == -1)
        errExit("epoll_ctl");

    char buf[BUF_SIZE];
    struct epoll_event events[2];

    int ready;
    int num_read;
    while (1) {
        ready = epoll_wait(epfd, events, 2, -1);

        for (int i = 0; i < ready; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                if ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) == -1)
                    errExit("Failed/Partial read\n");
                if (write(master_fd, buf, num_read) != num_read)
                    errExit("Coudn't write to master fd");
            } else if (events[i].data.fd == master_fd) {
                if ((num_read = read(master_fd, buf, BUF_SIZE)) == -1)
                    exit(EXIT_SUCCESS);
                if (write(STDOUT_FILENO, buf, num_read) != num_read)
                    errExit("Failed/Partial write\n");
            }
        }
    }
}
