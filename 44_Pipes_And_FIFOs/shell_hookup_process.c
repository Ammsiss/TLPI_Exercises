/*
    The aim of this program is to (very) roughly replicate the process
    that the shell goes through when piping 2 processes together
*/

#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s command1 arg1 .. | command2 arg2 ..", argv[0]);

    int pipe_index = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "|") == 0)
            pipe_index = i;
    }

    if (pipe_index == 0) {
        fprintf(stderr, "Pipe character must be present.\n");
        exit(EXIT_FAILURE);
    }

    argv[pipe_index] = NULL; /* Make pipe NULL So that execvp will
                                work with the first command */

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        errExit("pipe");

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        if (close(pipe_fd[0]) == -1)
            errExit("close");

        if (pipe_fd[1] != STDOUT_FILENO) {
            if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
                errExit("dup2");
            if (close(pipe_fd[1]) == -1)
                errExit("close");
        }

        execvp(argv[1], &argv[1]);
        errExit("execvp");
    default:
        break;
    }

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        if (close(pipe_fd[1]) == -1)
            errExit("close");

        if (pipe_fd[0] != STDIN_FILENO) {
            if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
                errExit("dup2");
            if (close(pipe_fd[0]) == -1)
                errExit("close");
        }

        execvp(argv[pipe_index + 1], &argv[pipe_index + 1]);
        errExit("execvp");
    default:
        break;
    }

    if (close(pipe_fd[0]) == -1)
        errExit("close");
    if (close(pipe_fd[1]) == -1)
        errExit("close");

    if (waitpid(-1, NULL, 0) == -1)
        errExit("waitpid");
    if (waitpid(-1, NULL, 0) == -1)
        errExit("waitpid");

    exit(EXIT_SUCCESS);
}
