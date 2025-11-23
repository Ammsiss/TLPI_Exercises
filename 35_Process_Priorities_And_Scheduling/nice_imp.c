#include <errno.h>
#include <sys/resource.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

int main(int argc, char **argv)
{
    if (argc < 3)
        usageErr("%s adjustment COMMAND", argv[0]);

    int adjustment = getNum(argv[1], "Invalid argument.\n");
    int prio;

    errno = 0;
    if ((prio = getpriority(PRIO_PROCESS, 0)) == -1 && errno != 0)
        errExit("getpriority");

    if (setpriority(PRIO_PROCESS, 0, prio + adjustment) == -1)
        errExit("setpriority");

    errno = 0;
    if ((prio = getpriority(PRIO_PROCESS, 0)) == -1 && errno != 0)
        errExit("getpriority");

    printf("Changed nice value to %d, now execing...\n", prio);

    execvp(argv[2], &argv[2]);
    errExit("execvp");
}
