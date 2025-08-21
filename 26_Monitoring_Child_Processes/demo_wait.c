#include <string.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sec ...", argv[0]);

    for (int i = 1; i < argc; ++i) {
        int seconds = getNum(argv[i], NULL);

        switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
            printf("child %d started with PID %d, sleeping %d seconds\n",
                    i, getpid(), seconds);
            sleep(seconds);
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    pid_t childPid;
    int numDead = 0;
    while ((childPid = wait(NULL)) != -1) {
        ++numDead;

        printf("wait() returned child PID %d (numDead=%d)\n", childPid, numDead);
        continue;
    }
    if (errno == ECHILD)
        errExit("wait");

    printf("No more children - bye!\n");
}
