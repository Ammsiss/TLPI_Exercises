/*
    This program was created as a solution to TLPI exersise 20-3

    This program verifies the behaviour of the SA_RESETHAND flag
    by calling a signal with the flag set twice. It should sleep
    for 1 second on the initial call then as the flag causes the
    handler to be reset after the first call it should interupt
    during the second call.
*/


#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>

#include "../lib/error_functions.h"

void handler(int sig)
{
    (void) sig;

    sleep(1);
}

int main(void)
{
    struct sigaction sa = { 0 };
    sa.sa_flags |= SA_RESETHAND;
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Calling interupt signal with SA_RESETHAND flag.\n");
    if (raise(SIGINT) == -1)   /* calls custom handler */
        errExit("raise");

    printf("Calling interupt signal with SA_RESETHAND for the second time.");
    fflush(stdout);
    if (raise(SIGINT) == -1)   /* calls default handler */
        errExit("raise");

    printf("This should never execute!\n");
}
