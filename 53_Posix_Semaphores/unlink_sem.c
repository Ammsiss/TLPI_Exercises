#include "tlpi_hdr.h" // IWYU pragma: export

#include <semaphore.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s filename", argv[0]);

    if (sem_unlink(argv[1]) == -1)
        errExit("sem_unlink");

    exit(EXIT_SUCCESS);
}
