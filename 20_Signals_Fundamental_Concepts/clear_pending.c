#define _GNU_SOURCE

#include <stdlib.h>
#include <signal.h>

#include "error_functions.h"
#include "signal_functions.h"

int main(void)
{
    sigset_t blockMask;
    if (sigfillset(&blockMask) == -1)
        errExit("sigfillset");

    /* Block all signals */
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
        errExit("sigprocmask");

    if (raise(SIGUSR1) != 0)   /* SIGUSR1 is now pending */
        errExit("raise");

    sigset_t pendingMask;
    sigpending(&pendingMask);

    printf("Printing pending signals...\n");
    printSigset(stdout, "", &pendingMask);
    printf("\n");

    printf("Setting SIGUSR1 to SIG_IGN...\n\n");
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");

    sigpending(&pendingMask);
    printf("Printing pending signals...\n");
    printSigset(stdout, "", &pendingMask);

    exit(EXIT_SUCCESS);
}
