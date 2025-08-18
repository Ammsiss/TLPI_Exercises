#define _GNU_SOURCE

#include <time.h>
#include <signal.h>
#include <sys/time.h>

#include "../lib/error_functions.h"

void handler(int sig)
{
    time_t tm = time(NULL);

    printf("Timer elapsed!\n");
    printf("%s\n", ctime(&tm));
}

int main(void)
{
    time_t tm = time(NULL);
    printf("%s\n", ctime(&tm));
    printf("Enter 'p' to print remaining time.\n");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = &handler;

    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    struct itimerval itv;
    /* Initial time */
    itv.it_value.tv_sec = 5;
    itv.it_value.tv_usec = 0;
    /* Post intervalic time */
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &itv, NULL);

    char buf[1000];
    while (1) {   /* Wait for timer expiry */
        int numRead = read(STDIN_FILENO, &buf, 1);
        if (numRead == -1 && errno != EINTR)
            errExit("read");

        buf[numRead] = '\0';

        if (strcmp(buf, "p") == 0) {
            struct itimerval itv;
            getitimer(ITIMER_REAL, &itv);
            printf("Remaining Time: %ld.%ld\n", itv.it_value.tv_sec, itv.it_value.tv_usec);
        }
    }
}
