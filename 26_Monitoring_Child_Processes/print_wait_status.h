#define _GNU_SOURCE

#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

/* This function is not async signal safe */

void printWaitStatus(const char *msg, int status)
{
    if (msg != NULL)
        printf("%s", msg);

    if (WIFEXITED(status)) {
        printf("child exited normally, exit status %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child caught signal %d (%s)", WTERMSIG(status),
                strsignal(WTERMSIG(WTERMSIG(status))));
        if (WCOREDUMP(status))
            printf(" (core dumped)\n");
        else
            printf("\n");
    } else if (WIFSTOPPED(status)) {
        printf("child stopped by signal  %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("child continued\n");
    }
}
