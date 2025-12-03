#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h" // IWYU pragma: export
#include <sys/mman.h>

void printLockedMem(char *addr, int len, unsigned char *vec, int vec_size)
{
    if (mincore(addr, len, vec) == -1)
        errExit("mincore");

    for (int i = 0; i < vec_size; ++i) {
        if (vec[i] & 1)
            printf("*");
        else
            printf(".");
    }

    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc != 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pages group lock\n", argv[0]);

    int page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
        errExit("sysconf");

    int num_pages = getInt(argv[1], GN_GT_0, "num-pages");
    int group_size = getInt(argv[2], GN_GT_0, "group-size");
    int lock_len = getInt(argv[3], GN_GT_0, "lock-length");
    if (num_pages % group_size != 0)
        fatal("group size must be divisible by number of pages");
    if (lock_len > group_size)
        lock_len = group_size;

    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    char *addr = mmap(NULL, num_pages * page_size, prot, flags, -1, 0);
    if (addr == NULL)
        errExit("mmap");

    int vec_size = (num_pages * page_size + page_size - 1) / page_size;
    unsigned char *vec = malloc(vec_size);
    if (vec == NULL)
        errExit("malloc");

    printf("Memory before locking:\n");
    printLockedMem(addr, num_pages * page_size, vec, vec_size);

    for (int i = 0; i < num_pages / group_size; ++i)
        if (mlock(addr + (i * group_size * page_size), page_size * lock_len) == -1)
            errExit("mlock");

    printf("Memory after locking:\n");
    printLockedMem(addr, num_pages * page_size, vec, vec_size);

    exit(EXIT_SUCCESS);
}
