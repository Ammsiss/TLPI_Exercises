#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "../lib/error_functions.h"

timer_t tid;

void handler(int sig __attribute__((unused)), siginfo_t *si __attribute__((unused)),
    void *uc __attribute__((unused)))
{
    /* Unsafe functions used */

    if (tid == si->si_value.sival_ptr)
        printf("si_value.sival_ptr matches timer_t value!\n");
    else
        printf("si_value.sival_ptr doesn't match timer_t value!\n");
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    if (timer_create(CLOCK_MONOTONIC, NULL, &tid) == -1)
        errExit("timer_create");

    printf("Created timer with address: %p\n", tid);
    printf("Waiting for signals...\n");

    struct itimerspec timerVal;
    timerVal.it_value.tv_sec = 5;
    timerVal.it_value.tv_nsec = 0;
    timerVal.it_interval.tv_sec = 0;
    timerVal.it_interval.tv_nsec = 0;

    if (timer_settime(tid, 0, &timerVal, NULL) == -1)
        errExit("timer_setttime");

    while (1)
        pause();   /* Wait for signals */
}
