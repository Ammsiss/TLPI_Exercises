#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

#define BUF_SIZE 10

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("%s message\n", argv[0]);

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        errExit("pipe");

    switch(fork()) {
    case -1:
        errExit("fork");
    case 0:
    {
        /* Child closes write descriptor */
        if (close(pipe_fd[1]) == -1)
            errExit("close");

        char buf[BUF_SIZE + 1];

        int rv;             /* Keep looping until EOF */
        while ((rv = read(pipe_fd[0], buf, BUF_SIZE)) != 0) {
            if (rv == -1)
                errExit("read");

            /* null terminate then print as data comes in */
            buf[rv] = '\0';
            printf("%s", buf);
        }

        /* Final newline after all data is printed */
        printf("\n");

        /* Close read end of pipe and exit */
        if (close(pipe_fd[0]) == -1)
            errExit("close");

        exit(EXIT_SUCCESS);
    }
    default:
        break;
    }

    /* Parent closes read descriptor */
    if (close(pipe_fd[0]) == -1)
        errExit("close");

    int num_wrote = 0;
    int index = 0;
    int to_write = strlen(argv[1]);

    while (to_write != 0) {
        num_wrote = write(pipe_fd[1], argv[1] + index, to_write);
        if (num_wrote == -1)
            errExit("write");

        index += num_wrote;
        to_write -= num_wrote;
    }

    if (close(pipe_fd[1]) == -1)
        errExit("close");

    if (waitpid(-1, NULL, 0) == -1)
        errExit("waitpid");

    exit(EXIT_SUCCESS);
}
