#include <pthread.h>
#include <string.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

static volatile int global = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadFunc(void *arg)
{
    int iterations = *(int *) arg;

    int loc;
    for (int i = 0; i < iterations; ++i) {

        pthread_mutex_lock(&mutex);

        /* Critical section -- Start */
        loc = global;
        ++loc;
        global = loc;
        /* Critical section -- End */

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [iterations]", argv[0]);

    pthread_t thread1;
    pthread_t thread2;

    int iterations = 10000;
    if (argv[1] != NULL)
        iterations = getNum(argv[1], "Input must be an integer.\n");

    int s;
    if ((s = pthread_create(&thread1, NULL, &threadFunc, &iterations)) != 0)
        errExitEN(s, "pthread_create");
    if ((s = pthread_create(&thread2, NULL, &threadFunc, &iterations)) != 0)
        errExitEN(s, "pthread_create");

    if ((s = pthread_join(thread1, NULL)) != 0)
        errExitEN(s, "pthread_join");
    if ((s = pthread_join(thread2, NULL)) != 0)
        errExitEN(s, "pthread_join");

    printf("global value: %d\n", global);
    exit(EXIT_SUCCESS);
}
