#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"
#include "../26_Monitoring_Child_Processes/print_wait_status.h"

#define BUF_SIZE 255

int main(void)
{
    char command[255];

    while (1) {
        printf("Command: ");
        fflush(stdout);

        int numRead = read(STDIN_FILENO, command, 255);

        if (numRead == 1)   /* User just submitted '\n' */
            strcpy(command, "");
        else
            command[numRead - 1] = '\0';

        int system_rv = system(command);
        printf("system() returned: status=%d\n", system_rv);

        if (strcmp(command, "") == 0)
            printf("Shell is %savailable.\n", system_rv ? "" : "not ");

        switch(system_rv) {
        case -1:
            errExit("system");
            break;
        default:
            if (WIFEXITED(system_rv) && WEXITSTATUS(system_rv) == 127)
                printf("Shell could not be execed by child process.\n");
            else
                printWaitStatus(NULL, system_rv);
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
