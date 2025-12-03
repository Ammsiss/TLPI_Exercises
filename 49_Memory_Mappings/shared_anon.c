#include "../lib/tlpi_hdr.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
#ifdef USE_MAP_ANON
    int protections = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;
    int *addr = mmap(NULL, sizeof(int), protections, flags, -1, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");
#else
    int fd = open("/dev/zero", O_RDWR);
    if (fd == -1)
        errExit("open");
    int protections = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED; 
    int *addr = mmap(NULL, sizeof(int), protections, flags, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");
#endif

    *addr = 1;

    int childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        *addr = 2;
        _exit(EXIT_SUCCESS);
    default:
        break;
    }

    if (waitpid(childPid, NULL, 0) == -1)
        errExit("waitpid");
    printf("Value: %d\n", *addr);
    exit(EXIT_SUCCESS);
}
