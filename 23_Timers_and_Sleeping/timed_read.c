/*
    This program demonstrates a method in which you can set
    a timer for how long a blocking system calls hould block.
*/

#define _GNU_SOURCE

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include "../lib/error_functions.h"

static void readInt(int sig)
{
}

int main(void)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = &readInt;

    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    char buf[1000];
    while (1) {

        alarm(3);

        if (read(STDIN_FILENO, buf, 1000) == -1 && errno != EINTR)
            errExit("read");

        printf("Interupted read!\n");
    }
}
