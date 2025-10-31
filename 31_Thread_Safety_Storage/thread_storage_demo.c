#define GNU_SOURCE

#include <pthread.h>
#include <stdio.h>

#include "../lib/error_functions.h"

void *thread_start(void *arg)
{
    printf("Hello, World!\n");

    return NULL;
}

void dest(void *value)
{
}

int main(void)
{
    pthread_t thread;

    int s;
    if ((s = pthread_create(&thread, NULL, thread_start, NULL)) != 0)
        errExitEN(s, "pthread_create");

    if ((s = pthread_join(thread, NULL)) != 0)
        errExitEN(s, "pthread_join");

    exit(EXIT_SUCCESS);
}
