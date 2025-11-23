#define _GNU_SOURCE

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    pid_t pid = getpid();
    pid_t pgrp = getpgrp();

    printf("PID: %d, PGID: %d\n", pid, pgrp);
    if (pid == pgrp) {
        printf("Because pid and pgrp are equal this is a process leader.\n");
        printf("We will now call setsid().\n");
    } else
        exit(EXIT_FAILURE);   /* Shouldn't happen */

    if (setsid() == -1) {
        fprintf(stderr, "setsid() call failed!\n");
        fprintf(stderr, "errno: %d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Shouldn't happen */
    exit(EXIT_SUCCESS);
}
