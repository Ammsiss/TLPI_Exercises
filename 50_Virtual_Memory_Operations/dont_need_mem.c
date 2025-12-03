#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h" // IWYU pragma: export
#include <sys/mman.h>

void printBytes(char *addr, int len)
{
    for (int i = 0; i < len; ++i) {
        if (i % 8 == 0 && i != 0)
            printf("\n");

        if (addr[i] == '\0')
            printf("\\0");
        else
            printf("%c ", addr[i]);
    }

    printf("\n");
}

int main(void)
{
    int page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
        errExit("sysconf");

    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    char *addr = mmap(NULL, page_size, prot, flags, -1, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    memcpy(addr, "abcdefghijklmnop", 16);

    printf("Memory region before madvise() MADV_DONTNEED:\n\n");
    printBytes(addr, 16);

    madvise(addr, 16, MADV_DONTNEED);

    printf("\nMemory region after madvise() MADV_DONTNEED:\n\n");
    printBytes(addr, 16);

    exit(EXIT_SUCCESS);
}
