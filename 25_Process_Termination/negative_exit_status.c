#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

int main(void)
{
    int exitStatus;
    pid_t childPid;

    if (fflush(stdout) == EOF)
        errExit("fflush");
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        exit(-1);
    default:
        if ((exitStatus = wait(NULL)) == -1)
            errExit("wait");
        printf("Exit status: %d\n", exitStatus);
        exit(EXIT_SUCCESS);
    }
}
