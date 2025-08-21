#include <unistd.h>

#include "../lib/error_functions.h"

int main(void)
{
    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        sleep(5);   /* Wait for parent to exit */
        printf("Child PPID: %d\n", getppid());
        exit(EXIT_SUCCESS);
    default:
        printf("Parent exiting...\n");
        _exit(EXIT_SUCCESS);   /* Parent exits first */
    }
}
