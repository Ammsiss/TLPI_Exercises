/*
    36_Process_Resources main  > time ./test_rusage_flag.bin

    ./test_rusage_flag.bin  0.00s user 0.00s system       0% cpu 1.001 total
*/

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        usageErr("%s COMMAND", argv[0]);

    struct timeval tv_old;
    if (gettimeofday(&tv_old, NULL) == -1)
        errExit("gettimeofday");

    int childPid;
    switch (childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        execvp(argv[1], &argv[1]);
        errExit("execvp");
    default:
        break;
    }

    /* Uncomment to test acutally using cpu time with 'yes' command */

    // sleep(2);
    // kill(childPid, SIGINT);

    if (waitpid(childPid, NULL, 0) == -1)
        errExit("waitpid");

    struct timeval tv_new;
    if (gettimeofday(&tv_new, NULL) == -1)
        errExit("gettimeofday");

    struct rusage rinfo;
    if (getrusage(RUSAGE_CHILDREN, &rinfo) == -1)
        errExit("getrusage");

    /* Total clock time in seconds */
    double realTime = ((tv_new.tv_sec * 1000000 + tv_new.tv_usec) -
         (tv_old.tv_sec * 1000000 + tv_old.tv_usec)) / 1000000.0F;

    /* Total cpu time in microseconds */
    int totalCpu = rinfo.ru_stime.tv_sec * 1000000 + rinfo.ru_stime.tv_usec +
        rinfo.ru_stime.tv_sec * 1000000.0f + rinfo.ru_stime.tv_usec;

    int cpuPercent = 100 * ((totalCpu / 1000000.0) / realTime);

    printf("%s  %ld.%ld user %ld.%ld system %d%% cpu %f total\n", argv[1],
        rinfo.ru_utime.tv_sec, rinfo.ru_utime.tv_usec, rinfo.ru_stime.tv_sec,
        rinfo.ru_stime.tv_usec, cpuPercent, realTime);

    exit(EXIT_SUCCESS);
}
