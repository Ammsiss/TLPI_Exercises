#include "tlpi_hdr.h" // IWYU pragma: export
#include "pshm.h"

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>

int main(void)
{
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd, MMAP_SIZE) == -1)
        errExit("ftruncate");

    char *addr = mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE,
            MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    sem_t *write_sem = sem_open(WRITER_SEM, O_CREAT, 0600, 1);
    if (write_sem == SEM_FAILED)
        errExit("sem_open");

    sem_t *read_sem;
    while (1) {
        read_sem = sem_open(READER_SEM, 0);
        if (read_sem == SEM_FAILED) {
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

    char buf[MMAP_SIZE];

    while (1) {
        if (sem_wait(write_sem) == -1)
            errExit("sem_wait");

        int num_read = read(STDIN_FILENO, buf, MMAP_SIZE - 1);

        if (num_read == -1)
            errExit("num_read");
        if (num_read == 0) {
            if (ftruncate(fd, 0) == -1)
                errExit("ftruncate");
            break;
        }

        buf[num_read] = '\0';
        strcpy(addr, buf);

        if (sem_post(read_sem) == -1)
            errExit("sem_wait");
    }

    if (sem_post(read_sem) == -1)
        errExit("sem_wait");

    if (sem_unlink(WRITER_SEM) == -1)
        errExit("sem_unlink");
    if (shm_unlink(SHM_NAME) == -1)
        errExit("shm_unlink");
    return EXIT_SUCCESS;
}
