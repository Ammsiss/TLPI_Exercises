/*
*/

#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>

#include "../lib/error_functions.h"

void handler(int sig, siginfo_t *si, void *ucontext)
{
    /* Unsafe functions used */

    printf("Signal caught: %d (%s)\n", sig, strsignal(sig));
    printf("Pid: %d   Uid: %d\n", si->si_pid, si->si_uid);
    printf("si_code: %s\n", (si->si_code == SI_USER) ? "SI_USER" :
                            (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
    printf("Realtime signal: %s\n", (sig >= SIGRTMIN) ? "yes" : "no");
}

int main(void)
{
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler;

    /* Set handler for all signals */

    for (int sig = 1; sig < NSIG; ++sig)
        sigaction(sig, &sa, NULL);

    sigset_t blockMask;
    sigfillset(&blockMask);
    sigdelset(&blockMask, SIGINT);

    sigset_t oldMask;

    if (sigprocmask(SIG_SETMASK, &blockMask, &oldMask) == -1)
        errExit("sigprocmask");

    printf("Waiting...\n");
    sleep(30);

    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) == -1)
        errExit("sigprocmask");
}
