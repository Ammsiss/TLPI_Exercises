#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "../lib/error_functions.h"

void handler(int sig)
{
    printf("Signal Caught %d (%s)\n", sig, strsignal(sig));
}

int main(int argc, char **argv)
{
    if (argc > 2)
        usageErr("%s [x]", argv[0]);

    /* Exit on parent or error */
    if (fork() != 0) {
        sleep(1);
        exit(EXIT_SUCCESS);
    }

    setlinebuf(stdout);

    printf("Child PID: %d\n", getpid());

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (argc == 2 && argv[1][0] == 'x')
        sa.sa_handler = handler;
    else
        sa.sa_handler = SIG_DFL;

    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");

    /* Wait for one of the job control signals */
    sleep(60);
}
