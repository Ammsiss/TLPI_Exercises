#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>

#include "../lib/error_functions.h"

int main(void)
{
    long ticks = sysconf(_SC_CLK_TCK);
    if (ticks == -1)
        errExit("sysconf");

    /* Test one: using getcwd to store cwd as a string */

    for (int i = 0; i < 100000; ++i) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, PATH_MAX) == NULL)
            errExit("getcwd");

        if (chdir("/") == -1)
            errExit("chdir");

        if (chdir(cwd) == -1)
            errExit("chdir");
    }

    struct tms tms_buf;
    clock_t tm = times(&tms_buf);
    if (tm == -1)
        errExit("times");

    clock_t test_one_stime = tms_buf.tms_stime;
    clock_t test_one_utime = tms_buf.tms_utime;

    printf("Method 1 (Using buffer):\n");
    printf("System: %.4f   User: %.4f\n",
        (double) test_one_stime / ticks, (double) test_one_utime / ticks);

    /* Test two: storing the cwd as a file discriptor*/

    for (int i = 0; i < 100000; ++i) {

        int fd;
        if ((fd = open(".", O_RDONLY)) == -1)
            errExit("open");

        if (chdir("/") == -1)
            errExit("chdir");

        if (fchdir(fd) == -1)
            errExit("fchdir");

        close(fd);
    }

    tm = times(&tms_buf);
    if (tm == -1)
        errExit("times");

    clock_t test_two_stime = tms_buf.tms_stime - test_one_stime;
    clock_t test_two_utime = tms_buf.tms_utime - test_one_utime;

    printf("Method 2 (Using fd):\n");
    printf("System: %.4f   User: %.4f\n",
            (double) test_two_stime / ticks, (double) test_two_utime / ticks);

    exit(EXIT_SUCCESS);
}
