#include "tlpi_hdr.h" // IWYU pragma: export

#include <semaphore.h>
#include <pthread.h>

#define BUF_SIZE 4096

char data[BUF_SIZE];
int eof = 0;

static sem_t sem1;
static sem_t sem2;

static void *readerFunc(void *arg)
{
    (void) arg;

    while (1) {
        if (sem_wait(&sem2) == -1)
            errExit("sem_wait");

        if (eof)
            break;

        printf("%s", data);

        if (sem_post(&sem1) == -1)
            errExit("sem_wait");
    }

    return NULL;
}

int main(void)
{
    if (sem_init(&sem1, 0, 1) == -1)
        errExit("sem_init");
    if (sem_init(&sem2, 0, 0) == -1)
        errExit("sem_init");

    pthread_t thread;

    int ern;
    if ((ern = pthread_create(&thread, NULL, readerFunc, NULL)) != 0)
        errExitEN(ern, "pthread_create");

    while(1) {
        if (sem_wait(&sem1) == -1)
            errExit("sem_wait");

        int num_read = read(STDIN_FILENO, data, BUF_SIZE - 1);
        if (num_read == -1)
            errExit("read");
        data[num_read] = '\0';

        if (num_read == 0)
            eof = 1;

        if (sem_post(&sem2) == -1)
            errExit("sem_wait");

        if (eof)
            break;
    }

    if ((ern = pthread_join(thread, NULL)) != 0)
        errExitEN(ern, "pthread_create");
    return EXIT_SUCCESS;
}
