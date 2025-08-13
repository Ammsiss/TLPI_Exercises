/*
Name:
    imp_ abort

Snyopsis:
    void imp_abort(void);

Description:
    Terminate the calling process and produce a core dump file.

*/

#define _GNU_SOURCE

#include <signal.h>

#include "../lib/error_functions.h"

void imp_abort()
{
    sigset_t newMask;
    sigemptyset(&newMask);
    sigaddset(&newMask, SIGABRT);

    /* It is permissible to attempt to unblock a signal which is not blocked. */
    if (sigprocmask(SIG_UNBLOCK, &newMask, NULL) == -1)
        errExit("sigprocmask");

    struct sigaction oact;
    if (sigaction(SIGABRT, NULL, &oact) == -1)
        errExit("sigaction");

    if (oact.sa_handler == SIG_IGN) {   /* If ignored, set to default */
        oact.sa_handler = SIG_DFL;
        oact.sa_flags = 0;
        sigemptyset(&oact.sa_mask);

        if (sigaction(SIGABRT, &oact, NULL) == -1)
            errExit("sigaction");
    }

    raise(SIGABRT);

    /* If the handler returns then set to SIG_DFL and raise again */

    oact.sa_handler = SIG_DFL;
    oact.sa_flags = 0;
    sigemptyset(&oact.sa_mask);
    if (sigaction(SIGABRT, &oact, NULL) == -1)
        errExit("sigaction");

    raise(SIGABRT);

    _exit(EXIT_FAILURE);
}

void handler(int sig)
{
    return;
}

int main(void)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = &handler;

    if (sigaction(SIGABRT, &sa, NULL) == -1)
        errExit("sigaction");

    imp_abort();

    while (1)   /* Should never execute (Unless longjmp) */
        ;
}
