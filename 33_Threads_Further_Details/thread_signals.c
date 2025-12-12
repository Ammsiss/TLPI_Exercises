#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "../lib/error_functions.h"

void printSigSet(sigset_t *set)
{
    int cnt = 0;
    for (int i = 1; i < NSIG; ++i) {
        if (sigismember(set, i)) {
            ++cnt;
            printf("%d (%s)\n", i, strsignal(i));
        }
    }
    if (cnt == 0)
        printf("Empty set.\n");
}

void *threadFunc(void *arg)
{
    (void) arg;

    sleep(1);

    sigset_t threadMask;
    if (sigemptyset(&threadMask) == -1)
        errExit("sigemptyset");
    if (sigpending(&threadMask) == -1)
        errExit("sigpending");

    printSigSet(&threadMask);

    return NULL;
}

int main(void)
{
    int s;

    pthread_t thr1;
    pthread_t thr2;

    sigset_t block_set;

    if (sigemptyset(&block_set) == -1)
        errExit("sigemptyset");
    if (sigaddset(&block_set, SIGINT) == -1)
        errExit("sigaddset");

    if (sigprocmask(SIG_SETMASK, &block_set, NULL) == -1)
        errExit("sigprocmask");

    if ((s = pthread_create(&thr1, NULL, threadFunc, NULL)) != 0)
        errExitEN(s, "pthread_create");

    if (sigemptyset(&block_set) == -1)
        errExit("sigemptyset");
    if (sigaddset(&block_set, SIGQUIT) == -1)
        errExit("sigaddset");

    if (sigprocmask(SIG_SETMASK, &block_set, NULL) == -1)
        errExit("sigprocmask");

    if ((s = pthread_create(&thr2, NULL, threadFunc, NULL)) != 0)
        errExitEN(s, "pthread_create");

    // Send blocked signals
    if ((s = pthread_kill(thr1, SIGINT)) != 0)
        errExitEN(s, "pthread_kill");
    if ((s = pthread_kill(thr2, SIGQUIT)) != 0)
        errExitEN(s, "pthread_kill");

    // Join threads
    if ((s = pthread_join(thr1, NULL)) != 0)
        errExitEN(s, "pthread_join");
    if ((s = pthread_join(thr2, NULL)) != 0)
        errExitEN(s, "pthread_join");
}
