#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>

#include "../lib/error_functions.h"

int main(void)
{
    char template[] = "/tmp/tempXXXXXX";
    int fd = mkstemp(template);
    if (fd == -1)
        errExit("mkstemp");

    setbuf(stdout, NULL);   /* Disable stdout buffering */

    printf("Attempting write on fd...");
    if (write(fd, "Hello, ", 7) == -1)
        errExit("write");
    else
        printf("Success!\n");

    pid_t childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:   /* Child */
        if (close(fd) == -1)   /* Child closes its copy of the fd but... */
            errExit("close");
        _exit(EXIT_SUCCESS);
    default:   /* Parent */
        printf("Attempting write on fd...");
        if (write(fd, "World!\n", 7) == -1)   /* ...parent still has their copy */
            errExit("write");
        else
            printf("Success!\n");
        exit(EXIT_SUCCESS);
    }
}
