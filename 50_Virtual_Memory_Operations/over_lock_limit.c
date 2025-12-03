#include "../lib/tlpi_hdr.h" // IWYU pragma: export
#include <sys/resource.h>
#include <sys/mman.h>

int main(void)
{
    struct rlimit rlim;
    if (getrlimit(RLIMIT_MEMLOCK, &rlim) == -1)
        errExit("getrlimit");
    printf("Current hard: %lu pages  Current soft: %lu pages\n",
            rlim.rlim_max / 4096, rlim.rlim_cur / 4096);

    rlim.rlim_cur = 8192;
    if (setrlimit(RLIMIT_MEMLOCK, &rlim) == -1)
        errExit("getrlimit");
    printf("New hard: %lu pages   New soft: %lu pages\n\n",
            rlim.rlim_max / 4096, rlim.rlim_cur / 4096);

    int page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
        errExit("sysconf");

    int prot = PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_PRIVATE;
    char *addr = mmap(NULL, page_size * 3, prot, flags, -1, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    printf("Attempting to lock 1 page...\n");
    if (mlock(addr, page_size) == -1)
        errExit("mlock");
    printf("    Success!\nAttempting to lock 1 page...\n");
    if (mlock(addr + page_size, page_size) == -1)
        errExit("mlock");
    printf("    Success!\nAttempting to lock 1 page...\n");
    if (mlock(addr + (page_size * 2), page_size) == -1)
        errExit("mlock");
    printf("    Success!\n");

    exit(EXIT_SUCCESS);
}
