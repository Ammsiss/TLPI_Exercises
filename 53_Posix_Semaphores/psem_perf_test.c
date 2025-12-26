#include "tlpi_hdr.h" // IWYU pragma: export

#include <semaphore.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s iterations", argv[0]);

    int iterations = getInt(argv[1], GN_GT_0, "iterations");

    sem_t sem;
    if (sem_init(&sem, 0, 0) == -1)
        errExit("sem_init");

    /* Uncontended calls should be faster then System V */

    for (int i = 0; i < iterations; ++i) {
        if (sem_post(&sem) == -1)
            errExit("sem_post");
        if (sem_wait(&sem) == -1)
            errExit("sem_wait");
    }

    return EXIT_SUCCESS;
}
