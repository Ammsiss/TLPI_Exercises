#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../26_Monitoring_Child_Processes/print_wait_status.h"

int imp_system(const char *command)
{
    int status;
    int childPid;

    switch (childPid = fork()) {
    case -1:
        return -1;
    case 0:
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        _exit(127);
    default:
        if (waitpid(childPid, &status, 0) == -1)
            return -1;
        else
            return status;
    }
}

int main(void)
{
    int status;
    if ((status = imp_system("ls -l")) != -1)
        printWaitStatus(NULL, status);

    exit(EXIT_SUCCESS);
}
