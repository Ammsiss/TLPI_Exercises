#define _GNU_SOURCE

#include <unistd.h>
#include <signal.h>

#include "../lib/error_functions.h"

#define CMD_BUF 255

int main(int argc, char **argv)
{
    printf("Parent PID=%d\n", getpid());

    char cmd[CMD_BUF];      /* Only works if basename is not truncated */
    snprintf(cmd, CMD_BUF, "ps | grep %s", basename(argv[0]));

    int childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child PID=%d, Exiting...\n", getpid());
        _exit(EXIT_SUCCESS);
    default:
        sleep(2);  /* ensure that child exits */
        if (system(cmd) == -1)
            errExit("system");
        if (kill(childPid, SIGKILL) == -1)
            errExit("kill");

        sleep(2);   /* ensure child receives signal */
        printf("ps after sending SIGKILL to PID=%d:\n", childPid);

        if (system(cmd) == -1)
            errExit("system");

        exit(EXIT_SUCCESS);
    }
}
