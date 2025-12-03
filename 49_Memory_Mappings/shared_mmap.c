#define _GNU_SOURCE

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../lib/tlpi_hdr.h" // IWYU pragma: export

int main(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file string\n", argv[0]);

    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1)
        errExit("open");

    struct stat sb;
    if (fstat(fd, &sb) == -1)
        errExit("fstat");
    if (sb.st_size == 0)
        fatal("Empty input file");
    if ((int) strlen(argv[2]) > sb.st_size)
        fatal("Input string must not be larger then file");

    char *addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");
    if (close(fd) == -1)
        errExit("close");

    printf("Current string=%.*s\n", (int) sb.st_size, addr);
    memset(addr, 0, sb.st_size);
    dprintf(fd, "%s", argv[2]);
    if (msync(addr, sb.st_size, MS_SYNC) == -1)
        errExit("msync");

    exit(EXIT_SUCCESS);
}
