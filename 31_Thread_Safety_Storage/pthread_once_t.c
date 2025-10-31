#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/error_functions.h"

#define THREAD_COUNT 100

pthread_once_t once = PTHREAD_ONCE_INIT;

/* When 'glob' is 1 it is initialized so init_func() should not be called again */
static int glob = 0;

void init_func(void) {
    if (glob == 1) {
        printf("The world just exploded.\n");
        return;
    }

    printf("glob initialized!\n");
    glob = 1;
}

void *start_func(void *arg) {
    long loop_count = (long) arg;

    for (int i = 0; i < loop_count; ++i)
        pthread_once(&once, &init_func);

    return NULL;
}

int main(void) {

    int s;

    pthread_t thread[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; ++i)
        if ((s = pthread_create(&thread[i], NULL, &start_func, (void *) 20)) != 0)
            errExitEN(s, "pthread_create");

    for (int i = 0; i < THREAD_COUNT; ++i)
        if ((s = pthread_join(thread[i], NULL)) != 0)
            errExitEN(s, "pthread_join");

    exit(EXIT_SUCCESS);
}
