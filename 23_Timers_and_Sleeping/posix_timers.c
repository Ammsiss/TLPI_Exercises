#define _GNU_SOURCE

#include <stdio.h>
#include <bits/types/sigevent_t.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include "../lib/error_functions.h"

volatile sig_atomic_t value = -1;

void handler(int sig, siginfo_t *si, void *ucontext)
{
    return;
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = &handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
        errExit("sigaction");

    struct sigevent evp;

    /* Prior to calling timer_create(), sigev_value and sigev_notify
       must be set. If SIGEV_SIGNAL is set you must also set sigev_signo */

    char payload[] = "This is a payload!\n";
    evp.sigev_value.sival_ptr = payload;
    evp.sigev_value.sival_int = 69;

    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGRTMIN;

    timer_t tid;
    if (timer_create(CLOCK_REALTIME, &evp, &tid) == -1)
        errExit("timer_create");

    struct itimerspec newValue;
    newValue.it_value.tv_sec = 5;
    newValue.it_value.tv_nsec = 0;
    newValue.it_interval.tv_sec = 2;
    newValue.it_interval.tv_nsec = 0;

    if (timer_settime(tid, 0, &newValue, NULL) == -1)
        errExit("timer_create");

    struct timespec sleepTime;
    sleepTime.tv_sec = 1;
    sleepTime.tv_nsec = 500000000;

    int intCount = 0;
    while (intCount < 5) {
        nanosleep(&sleepTime, NULL);

        struct itimerspec displayValue;
        if (timer_gettime(tid, &displayValue) == -1)
            errExit("timer_gettime");

        printf("Seconds left: %f\n", displayValue.it_value.tv_sec +
                            (double) displayValue.it_value.tv_nsec / 1000000000);

        sleep(10);

        printf("Interuptted!\n");
        ++intCount;
    }
    intCount = 0;

    newValue.it_value.tv_sec = 0;
    newValue.it_value.tv_nsec = 0;
    if (timer_settime(tid, 0, &newValue, NULL) == -1)
        errExit("timer_create");

    if (timer_delete(tid) == -1)
        errExit("timer_delete");

    printf("Waiting to see if a signal shows up...\n");
    sleep(10);
}
