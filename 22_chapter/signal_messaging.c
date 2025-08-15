#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/error_functions.h"

volatile sig_atomic_t newMessage = 0;

char mess[1000];

void handler(int sig, siginfo_t *si, void *ucontext)
{
    newMessage = 1;

    char letter[2];
    letter[0] = si->si_value.sival_int;
    letter[1] = '\0';
    strcat(mess, letter);
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

                for ( char *startMes = end + 1; *startMes != '\0'; ++startMes) {
                    sv.sival_int = *startMes;
                    if (sigqueue(pid, SIGRTMIN, sv) == -1)
                        errExit("sigqueue");
                }
            }
        }

        if (newMessage) {   /* sleep() could help if losing data */
            printf("Message: %s\n", mess);
            newMessage = 0;
            strcpy(mess, "");
        }
    }

    exit(EXIT_SUCCESS);
}
