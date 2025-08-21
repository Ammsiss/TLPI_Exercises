/*
    This program demonstrates the use of waitpid(). The parent process
    creates a child that either returns immediately or loops while waiting
    for signals. When the child process is termianted, stopped or continued
    the parent prints information returned by wait pid.

Usage: child_status [exitstatus]

    If exitstatus is passed in, child does not loop but instead
    returns immediately with the value of exitstatus.
*/

#define _GNU_SOURCE

#include <sys/wait.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"
#include "print_wait_status.h"

int main(int argc, char **argv)
{
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [sec]", argv[0]);

    int exitStatus = -1;
    if (argc == 2)
        exitStatus = getNum(argv[1], "Argument must be a number");

    int status;
    int waitrv;

    int childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        printf("Child started with pid %d\n", getpid());

        if (exitStatus != -1)
            _exit(exitStatus);
        else
            while (1)
                pause();   /* Wait for signals */
    default:
        while (1) {
            if ((waitrv = waitpid(childPid, &status, WCONTINUED | WUNTRACED)) == -1)
                break;

            printf("waitpid() -> (PID %d, status %d)\n", waitrv, status);
            printWaitStatus(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                break;
        }
        break;
    }
    if (errno != ECHILD)
        errExit("waitpid");

    exit(EXIT_SUCCESS);
}
