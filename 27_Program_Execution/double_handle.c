#define _GNU_SOURCE

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

void handler(int sig)
{
    printf("Handler invoked!\n");
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Blocking SIGCHLD...\n");

    sigset_t oldMask;
    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &blockMask, &oldMask) == -1)
        errExit("sigprocmask");

    // if (system("ls -l") == -1)
    //     errExit("system");

    int status;
    pid_t childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child exiting...\n");
        _exit(EXIT_SUCCESS);
    default:
        wait(&status);
        break;
    }

    printf("Unblocking SIGCHLD...\n");

    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) == -1)
        errExit("sigprocmask");
}
