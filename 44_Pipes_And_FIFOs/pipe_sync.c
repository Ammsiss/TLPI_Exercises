#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sleep-time ...", argv[0]);

    setbuf(stdout, NULL);  /* Make stdout unbuffered becuase we
                              terminate the child with _exit() */
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        errExit("pipe");

    for (int i = 0; i < argc - 1; ++i) {
        switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
        {
            if (close(pipe_fd[0]) == -1)
                errExit("close");

            /* Simulate some work that has to be done before the parent continues */
            int sleep_time = getNum(argv[i + 1], "Arguments must be integers!\n");
            sleep(sleep_time);

            printf("Child #%d finished work. Now closing write end of pipe.\n", i + 1);

            /* Done the work so close write end of the pipe */
            if (close(pipe_fd[1]) == -1)
                errExit("close");

            _exit(EXIT_SUCCESS);
        }
        default:
            break;
        }
    }

    /* close write end as to not get blocked for ever */
    if (close(pipe_fd[1]) == -1)
        errExit("close");

    char c;

    /* read blocks if there is nothing in the pipe and there are still
     * processes with open write fd's open */
    printf("Parent waiting at read() call...\n");
    read(pipe_fd[0], &c, 1);
    printf("Parent woke up! Now I can do my work.\n");

    while (1) {
        int rv = waitpid(-1, NULL, 0);

        if (rv == -1) {
            if (errno == ECHILD)
                break;
            else if (errno == EINTR)
                continue;
            else
                errExit("waitpid");
        }
    }

    exit(EXIT_SUCCESS);
}
