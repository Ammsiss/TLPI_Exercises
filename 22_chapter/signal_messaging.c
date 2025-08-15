#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

volatile sig_atomic_t newMessage = 0;
volatile sig_atomic_t message = 0;

void handler(int sig, siginfo_t *si, void *ucontext)
{
    newMessage = 1;
    message = si->si_value.sival_int;
}

int main(void)
{
    printf("Pid: %d\n", getpid());

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler;

    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
        errExit("sigaction");

    char buf[1000];

    while (1) {
        errno = 0;

        int numRead = read(STDOUT_FILENO, buf, 1000);
        if (numRead == -1 && errno != EINTR)
            errExit("read");

        if (errno != EINTR) {
            buf[numRead - 1] = '\0';

            char *end;
            pid_t pid = strtol(buf, &end, 10);
            if (*end != ':') {
                printf("Message format: <pid>:<message>\n");
                continue;
            } else {
                union sigval sv;
                sv.sival_int = getNum(end + 1);
                if (sigqueue(pid, SIGRTMIN, sv) == -1)
                    errExit("sigqueue");
            }
        }

        if (newMessage) {
            printf("Message: %d\n", message);
            newMessage = 0;
        }
    }

    exit(EXIT_SUCCESS);
}
