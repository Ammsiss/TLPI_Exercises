#include "tlpi_hdr.h" // IWYU pragma: export
#include "pshm.h"

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>

int main(void)
{
    sem_t *read_sem = sem_open(READER_SEM, O_CREAT, 0600, 0);
    if (read_sem == SEM_FAILED)
        errExit("sem_open");

    sem_t *write_sem;
    while (1) {
        write_sem = sem_open(WRITER_SEM, 0);
        if (write_sem == SEM_FAILED) {
            if (errno != ENOENT)
                errExit("sem_open");
            putchar('.');
            fflush(stdout);
            sleep(1);
            continue;
        }
        printf("\n");
        break;
    }

    int fd = shm_open(SHM_NAME, O_RDONLY, 0600);
    if (fd == -1)
        errExit("shm_open");

    char *addr = mmap(NULL, MMAP_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    struct stat sb;
    char buf[MMAP_SIZE];

    while (1) {
        if (sem_wait(read_sem) == -1)
            errExit("sem_wait");

        if (fstat(fd, &sb) == -1)
            errExit("fstat");
        if (sb.st_size == 0)
            break;

        strcpy(buf, addr);
        printf("%s", buf);

        if (sem_post(write_sem) == -1)
            errExit("sem_wait");
    }

    if (sem_unlink(READER_SEM) == -1)
        errExit("sem_unlink");
    return EXIT_SUCCESS;
}
