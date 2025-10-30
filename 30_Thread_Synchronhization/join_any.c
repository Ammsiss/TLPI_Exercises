#include <pthread.h>
#include <stdlib.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

typedef enum {
    UNJOINED,
    JOINED,
    ALIVE
} ThreadState;

typedef struct {
    pthread_mutex_t *mtx;
    pthread_t tid;
    ThreadState state;
    int sleep_time;
} ThreadInfo;

static ThreadInfo *thread_list;
static int total_unjoined = 0;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_func(void *arg)
{
    long index = (long) arg;
    long sleep_time = thread_list[index].sleep_time;

    sleep(sleep_time);

    int s;
    if ((s = pthread_mutex_lock(&mtx)) != 0)
        errExitEN(s, "pthread_mutex_lock");

    ++total_unjoined;

    if ((s = pthread_mutex_unlock(&mtx)) != 0)
        errExitEN(s, "pthread_mutex_unlock");

    if ((s = pthread_mutex_lock(thread_list[index].mtx)) != 0)
        errExitEN(s, "pthread_mutex_lock");

    thread_list[index].state = UNJOINED;

    if ((s = pthread_mutex_unlock(thread_list[index].mtx)) != 0)
        errExitEN(s, "pthread_mutex_unlock");

    if ((s = pthread_cond_signal(&cond)) != 0)
        errExitEN(s, "pthread_cond_signal");

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        usageErr("%s thread...", argv[0]);

    int s;

    thread_list = malloc(sizeof(ThreadInfo) * (argc - 1));

    for (long i = 0; i < argc - 1; ++i) {

        thread_list[i].state = ALIVE;
        thread_list[i].sleep_time = getNum(argv[i + 1], NULL);
        thread_list[i].mtx = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(thread_list[i].mtx, NULL);

        if ((s = pthread_create(&thread_list[i].tid, NULL, &thread_func, (void *) i)) != 0)
            errExitEN(s, "pthread_create");
    }

    int total_threads = argc - 1;
    int total_joined = 0;

    while (1) {
        if ((s = pthread_mutex_lock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_lock");

        while (total_unjoined == 0)
            if ((s = pthread_cond_wait(&cond, &mtx)) != 0)
                errExitEN(s, "pthread_cond_wait");

        for (int i = 0; i < total_threads; ++i) {

            if ((s = pthread_mutex_lock(thread_list[i].mtx)) != 0)
                errExitEN(s, "pthread_mutex_lock");

            if (thread_list[i].state == UNJOINED) {
                if ((s = pthread_join(thread_list[i].tid, NULL)) != 0)
                    errExitEN(s, "pthread_join");
                else
                    printf("Thread joined!\n");

                thread_list[i].state = JOINED;

                --total_unjoined;
                ++total_joined;
            }

            if ((s = pthread_mutex_unlock(thread_list[i].mtx)) != 0)
                errExitEN(s, "pthread_mutex_unlock");
        }

        if ((s = pthread_mutex_unlock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_unlock");

        if (total_joined == total_threads)
            break;
    }

    for (int i = 0; i < argc - 1; ++i) {
        if ((s = pthread_mutex_destroy(thread_list[i].mtx)) != 0)
            errExitEN(s, "pthread_mutex_destroy");
        free(thread_list[i].mtx);
    }

    free(thread_list);
}
