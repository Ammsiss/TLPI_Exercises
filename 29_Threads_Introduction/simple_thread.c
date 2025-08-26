#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "../lib/error_functions.h"

static int x = 1;
static int y = 69;

void *start(void *arg)
{
    printf("Thread started!\n");
    printf("Thread start function arg value: %d\n", *(int *) arg);
    sleep(1);
    printf("Thread exiting!\n");

    return &x;
}

int main(void)
{
    printf("Main thread creating thread.\n");

    pthread_t thread;

    int s;
    if ((s = pthread_create(&thread, NULL, &start, &y)) != 0)
        errExitEN(s, "pthread_create");

    printf("Main thread joining...\n");

    void *rv;
    if ((s = pthread_join(thread, &rv)) != 0)
        errExitEN(s, "pthread_join");

    printf("pthread_join() rv: %d\n", *(int *) rv);

    exit(EXIT_SUCCESS);
}
