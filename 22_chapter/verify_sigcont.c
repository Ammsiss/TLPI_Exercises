/*
    This program was created as a solution to TLPI exersise 22-1

    This Program shows that the main behaviour of SIGCONT (Resuming a process)
    can never be blocked. Blocking SIGCONT only has the behaviour of not running
    the handler until it is later unblocked. This is shown by stopping the process
    and waiting for SIGCONT, when it arrives the process resumes but does not execute
    the handler until the signal is later unblocked.
*/

#define _GNU_SOURCE

#include <signal.h>
#include <string.h>
#include <stdio.h>

#include "../lib/error_functions.h"

void handler(int sig, siginfo_t *si, void *ucontext)
{
    printf("Caught signal: %d (%s)\n", sig, strsignal(sig));
    printf("Senders uid/pid: %d   %d\n", si->si_uid, si->si_pid);
}

int main(void)
{
    /* As a convience print pid */
    printf("Process ID: %d\n", getpid());

    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCONT);

    sigset_t oldMask;

    /* Block SIGCONT */
    if (sigprocmask(SIG_BLOCK, &blockMask, &oldMask) == -1)
        errExit("sigprocmask");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler;

    if (sigaction(SIGCONT, &sa, NULL) == -1)
        errExit("sigaction");

    /* Stop until SIGCONT arrives then kill some time to show
       that the SIGCONT handler doesn't execute immediately. */

    raise(SIGSTOP);

    printf("Waiting...\n");
    sleep(3);

    /* Unblock SIGCONT */
    if (sigprocmask(SIG_SETMASK, &oldMask, NULL) == -1)
        errExit("sigprocmask");

    exit(EXIT_SUCCESS);
}
