#define _GNU_SOURCE

#include <sys/wait.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

/* If the WNOHANG flag is specified remember to 0 out the siginfo_t structure
   before using it */

void printWaitStatus(const char *msg, siginfo_t *info)
{
    if (msg != NULL)
        printf("%s", msg);

    switch (info->si_code) {
    case CLD_EXITED:
        printf("child exited normally, status %d\n", info->si_status);
        break;
    case CLD_KILLED:
        printf("child caught signal %d (%s)\n", info->si_status, strsignal(info->si_status));
        break;
    case CLD_DUMPED:
        printf("child caught signal %d (%s) (core dumped)\n",
            info->si_status, strsignal(info->si_status));
        break;
    case CLD_STOPPED:
        printf("child stopped by signal %d\n", info->si_status);
        break;
    case CLD_CONTINUED:
        printf("child continued by signal %d\n", info->si_status);
        break;
    default:
        printf("Unhandled case...\n");
        break;
    }
}

int main(int argc, char **argv)
{
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [sec]", argv[0]);

    int exitStatus = -1;
    if (argc == 2)
        exitStatus = getNum(argv[1], "Argument must be a number");

    siginfo_t info;
    int waitrv;

    int childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child started with pid %d\n", getpid());

        if (exitStatus != -1)
            _exit(exitStatus);
        else
            while (1)
                pause();   /* Wait for signals */
    default:
        while (1) {
            int opts = WEXITED | WSTOPPED | WCONTINUED;
            if ((waitrv = waitid(P_PID, childPid, &info, opts)) == -1)
                break;

            printf("waitpid() -> (PID %d)\n", waitrv);
            printWaitStatus(NULL, &info);

            if (info.si_code == CLD_EXITED || info.si_code == CLD_KILLED)
                break;
        }
        break;
    }
    if (errno != ECHILD)
        errExit("waitid");

    exit(EXIT_SUCCESS);
}
