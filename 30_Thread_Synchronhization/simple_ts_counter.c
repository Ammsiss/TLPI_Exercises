#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

#define LOOP_COUNT 10000

static int sum = 0;
pthread_mutex_t *mtx;

void ts_counter()
{
    int s;
    if ((s = pthread_mutex_lock(mtx)) != 0)
        errExitEN(s, "pthread_mutex_lock");

    int temp = sum;
    ++temp;
    sum = temp;

    if ((s = pthread_mutex_unlock(mtx)) != 0)
        errExitEN(s, "pthread_mutex_lock");
}

void *start_func(void *value)
{
    int loops = (long) value;

    for (int i = 0; i < loops; ++i)
        ts_counter();

    return NULL;
}

int main(void)
{
    int s;

    mtx = malloc(sizeof(pthread_mutex_t));
    if (mtx == NULL)
        errExit("malloc");
    pthread_mutex_init(mtx, NULL);

    pthread_t thread1;
    if ((s = pthread_create(&thread1, NULL, &start_func, (void *) LOOP_COUNT)) != 0)
        errExitEN(s, "pthread_create");

    pthread_t thread2;
    if ((s = pthread_create(&thread2, NULL, &start_func, (void *) LOOP_COUNT)) != 0)
        errExitEN(s, "pthread_create");

    pthread_t thread3;
    if ((s = pthread_create(&thread3, NULL, &start_func, (void *) LOOP_COUNT)) != 0)
        errExitEN(s, "pthread_create");

    if ((s = pthread_join(thread1, NULL)) != 0)
        errExitEN(s, "pthread_join");
    if ((s = pthread_join(thread2, NULL)) != 0)
        errExitEN(s, "pthread_join");
    if ((s = pthread_join(thread3, NULL)) != 0)
        errExitEN(s, "pthread_join");

    printf("The sum should be %d, what it is is %d\n", LOOP_COUNT * 3, sum);

    pthread_mutex_destroy(mtx);
    free(mtx);

    if (LOOP_COUNT * 3 != sum)
        exit(EXIT_FAILURE);
    else
        exit(EXIT_SUCCESS);
}
