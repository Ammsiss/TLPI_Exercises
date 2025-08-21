#define _GNU_SOURCE

#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

#define ALRT_SIG SIGUSR1

pid_t parentPid;

void handler(int sig)
{
    return;
}

void grandChild()
{
    sleep(3);   /* Wait for parent to exit (also lets parent get to sigsuspend) */
    printf("Grand child started. PID=%d PPID=%d\n", getpid(), getppid());

    if (kill(parentPid, ALRT_SIG) == -1)   /* alert grandpa that its time to call wait() */
        errExit("kill");

    sleep(3);   /* wait for grandpa to call wait() */

    printf("Grand child: PID=%d PPID=%d\n", getpid(), getppid());
    int status;
    printf("Grand child leaving...\n");
}

void child()
{
    printf("Child started. PID=%d PPID=%d\n", getpid(), getppid());

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        grandChild();
        _exit(EXIT_SUCCESS);
    default:
        printf("Child exiting...\n");
        _exit(EXIT_SUCCESS);
    }
}

int main(void)
{
    parentPid = getpid();

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = &handler;
    sa.sa_flags = 0;

    if (sigaction(ALRT_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    sigset_t emptyMask;
    sigemptyset(&emptyMask);

    pid_t childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        child();
        _exit(EXIT_SUCCESS);
    default:
        printf("Parent waiting for signal. (PID=%d)\n", getpid());
        sigsuspend(&emptyMask);

        printf("Parent calling wait()\n");
        int rv;
        while ((rv = waitpid(childPid, NULL, WNOHANG)) == 0)
            continue;
        if (rv == -1)
            errExit("waitpid");

        sleep(5);   /* Wait for rest of output */

        printf("Parent exiting...\n");
        exit(EXIT_SUCCESS);
    }
}
