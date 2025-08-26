#define _GNU_SOURCE

#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

int main(int argc, char **argv)
{
    if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [calls]", argv[0]);

    if (malloc(10000000) == NULL)
        errExit("malloc");

    int callCount = 100000;
    if ((argv[1]) != NULL)
        callCount = getNum(argv[1], "Input must be a integer.\n");

    int childPid;

    for (int i = 0; i < callCount; ++i) {
#ifdef USE_VFORK
        switch (childPid = vfork()) {
#else
        switch(childPid = fork()) {
#endif
        case -1:
            errExit("fork/vfork");
        case 0:
            _exit(EXIT_SUCCESS);   /* Child imediately exits */
        default:
            waitpid(childPid, NULL, 0);
            break;
        }
    }
}
