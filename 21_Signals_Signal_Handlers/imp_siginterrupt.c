/*
Name:
    imp_ siginterrupt

Synopsis:
    int siginterrupt(int sig, int flag);

Description:
    Modifies the SA_RESTART flag for a given signal handler. If flag
    is 1 the SA_RESTART is enabled if it is 0 it is disabled.

Return:
    0 on success, -1 and EINVAL if sig is invalid
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/error_functions.h"

void handler(int sig, siginfo_t *info, void *ucontext)
{
}

int imp_siginterrupt(int sig, int flag)
{
    struct sigaction osa;
    if (sigaction(sig, NULL, &osa) == -1)
        return -1;

    if (flag != 0)
        osa.sa_flags |= SA_RESTART;
    else
        osa.sa_flags &= ~SA_RESTART;

    if (sigaction(sig, &osa, NULL) == -1)
        return -1;

    return 0;
}

int main(void)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler;

    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    imp_siginterrupt(SIGINT, 1);   /* Turn on SA_RESTART */

    int fd = open("/dev/tty", O_RDONLY);

    char c;
    if (read(fd, &c, 1) == -1)
        errExit("read");

    close(fd);
}
