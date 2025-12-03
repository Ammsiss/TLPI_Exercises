#define _GNU_SOURCE

#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../lib/tlpi_hdr.h" // IWYU pragma: export

int main(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[0], "--help") == 0)
        usageErr("%s src dest\n", argv[0]);

    int src_fd = open(argv[1], O_RDWR);
    if (src_fd == -1)
        errExit("open");

    struct stat sb;
    if (fstat(src_fd, &sb) == -1)
        errExit("fstat");
    if (sb.st_size == 0)
        fatal("src file is empty");

    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED;
    char *src_addr = mmap(NULL, sb.st_size, prot, flags, src_fd, 0);
    if (src_addr == MAP_FAILED)
        errExit("mmap");

    if (close(src_fd) == -1)
        errExit("close");

    int dest_fd = open(argv[2], O_CREAT | O_EXCL | O_RDWR, 0660);
    if (dest_fd == -1)
        errExit("open");

    if (ftruncate(dest_fd, sb.st_size) == -1)
        errExit("ftruncate");

    char *dest_addr = mmap(NULL, sb.st_size, prot, flags, dest_fd, 0);
    if (dest_addr == MAP_FAILED)
        errExit("mmap");

    if (close(dest_fd) == -1)
        errExit("close");

    memcpy(dest_addr, src_addr, sb.st_size);

    if (munmap(src_addr, sb.st_size) == -1)
        errExit("munmap");
    if (munmap(dest_addr, sb.st_size) == -1)
        errExit("munmap");
    exit(EXIT_SUCCESS);
}
