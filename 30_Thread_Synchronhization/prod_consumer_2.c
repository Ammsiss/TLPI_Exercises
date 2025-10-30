#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static int avail = 0;

void *thread_start(void *arg)
{
    int id = rand() % 100 + 1;
    printf("Thread %d entered.\n", id);

    int sleep_time = rand() % 3 + 1;

    long iterations = (long) arg;

    for (int i = 0; i < iterations; ++i) {

        sleep(sleep_time);   // Simulate generating a unit

        int s;
        if ((s = pthread_mutex_lock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_lock");

        ++avail;

        if ((s = pthread_mutex_unlock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_unlock");

        if ((s = pthread_cond_signal(&cond)) != 0)
            errExitEN(s, "pthread_cond_signal");
    }

    printf("Thread %d exiting.\n", id);

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        usageErr("%s interations...", argv[0]);

    printf("total_threads = %d\n", argc - 1);

    int total_units = 0;

    pthread_t tid;
    for (int i = 1; i < argc; ++i) {
        long iterations = getNum(argv[i], "Input must be an integer!");
        total_units += iterations;

        pthread_create(&tid, NULL, &thread_start, (void *) iterations);
    }

    int tot_consumed = 0;

    int s;
    while (1) {
        if ((s = pthread_mutex_lock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_lock");

        while (avail == 0) {
            if ((s = pthread_cond_wait(&cond, &mtx)) != 0)
                errExitEN(s, "pthread_cond_wait");
        }

        while (avail > 0) {
            --avail;
            ++tot_consumed;
        }

        printf("Main thread signaled! avail = %d, tot_consumed = %d\n", avail, tot_consumed);

        if (tot_consumed >= total_units)
            break;

        if ((s = pthread_mutex_unlock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_unlock");
    }

    exit(EXIT_SUCCESS);
}
