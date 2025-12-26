#include "tlpi_hdr.h" // IWYU pragma: export

#include <semaphore.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s filename value", argv[0]);

    int sem_init = getInt(argv[2], GN_NONNEG, "value");

    if (sem_open(argv[1], O_CREAT | O_EXCL, 0600, sem_init) == SEM_FAILED)
        errExit("sem_open");

    exit(EXIT_SUCCESS);
}
