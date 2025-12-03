#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h" // IWYU pragma: export
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

void catcher(int sig)
{
    // Not async signal safe
    printf("Signal caught %d (%s)\n", sig, strsignal(sig));
    exit(EXIT_FAILURE);
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = catcher;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGBUS, &sa, NULL) == -1)
        errExit("sigaction");

    int fd;
    if((fd = open("t.txt", O_RDWR)) == -1)
        errExit("open");

    char *addr = mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    /* If access between 0 and 1999 It should be fine! */
    // if (write(STDOUT_FILENO, addr, 2000) == -1)
    //     errExit("write");

    /* If access between 2000 and 4095 It also should be fine! This is just
     * zeroed out by the kernel! */
    // if (write(STDOUT_FILENO, (addr + 2000), 2096) == -1)
    //     errExit("write");

    /* If access between 4096 and 8191 I should get SIGBUS because its within
     * the mapping but not currently mapped to the file */
    // char c = *(addr + 6000);
    // (void) c;

    /* If access between 8192 and beyond I should get SIGSEGV because its beyond
     * the end of the mapping! But you may not get SIGSEGV because if the address
     * you happen to land on is already used (maybe by the stdio library functions)
     * then itll just print whatever garbage is there. Play around with the offset
     * until you get a SIGSEGV! */
    char c = *(addr + 2000000);
    printf("%c\n", c);

    exit(EXIT_SUCCESS);
}
