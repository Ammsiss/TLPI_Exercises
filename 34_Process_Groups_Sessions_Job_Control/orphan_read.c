#define _GNU_SOURCE

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    /* Parent or error exits; child process is in a orphaned group */
    if (fork() != 0)
        exit(EXIT_SUCCESS);

    /* Make sure parent exits first */
    sleep(1);

    char c;
    if (read(STDIN_FILENO, &c, 1) == -1) {
        /* Must redirect to stdout to a file to see this (as its hooked up to
         * the controlling terminal by default) */
        printf("Read to stdin failed with errno: %s\n", strerror(errno));
    }
}
