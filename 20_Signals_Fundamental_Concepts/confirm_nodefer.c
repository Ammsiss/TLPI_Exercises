/*
    This program was created as a solution to TLPI exersise 20-3

    This program tests the SA_NODEFER flag by printing when
    execution enters and exits the handler. If the flag is not
    enabled then the handler should not recurse. The signal
    should be enabled in the pending mask and called very soon
    after the handler returns resulting in the output:

    Entered func!
    Left func!
    Entered func!
    Left func!

    If the flag is enabled then the handler should be called
    even when execution is within the handler resulting in the
    output:

    Entered func!
    Entered func!
    Left func!
    Left func!
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#include "error_functions.h"

int breakOut = 0;

void recurseHandler(int sig)
{
    (void) sig;

    printf("Entered func!\n");

    if (breakOut)
        return;

    breakOut = 1;

    if (raise(SIGUSR1) == -1)
        errExit("raise");
    printf("Left func!\n");
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = &recurseHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Calling signal without SA_NODEFER flag...\n");
    if (raise(SIGUSR1) == -1)   /* Sends SIGUSR1 inside handler, should be pending */
        errExit("raise");
    printf("Left func!\n");

    breakOut = 0;

    sa.sa_flags |= SA_NODEFER;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Calling signal with SA_NODEFER flag...\n");
    if (raise(SIGUSR1) == -1)
        errExit("raise");
    printf("Left func!\n");

    exit(EXIT_SUCCESS);
}
