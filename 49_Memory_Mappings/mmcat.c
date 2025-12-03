#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../lib/tlpi_hdr.h" // IWYU pragma: export

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    struct stat st;
    if (stat(argv[1], &st) == -1)
        errExit("stat");
    if (st.st_size == 0) {
        printf("Empty file\n");
        exit(EXIT_SUCCESS);
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1)
        errExit("open");

    char *addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    if (write(STDOUT_FILENO, addr, st.st_size) != st.st_size)
        fatal("partial/failed write");

    if (munmap(addr, st.st_size) == -1)
        errExit("munmap");

    exit(EXIT_SUCCESS);
}
