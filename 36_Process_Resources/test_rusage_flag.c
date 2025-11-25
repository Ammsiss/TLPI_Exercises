#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

int main(void)
{
    int childPid;
    switch(childPid = fork()) {
    case -1:
        errExit("fork");
    case 0:
        exit(EXIT_SUCCESS);
    default:
        sleep(1);   /* Make sure child dies */
    }

    struct rusage rinfo;

    /* Call before child has been waited for */
    if (getrusage(RUSAGE_CHILDREN, &rinfo) == -1)
        errExit("getrusage");

    printf("Cpu time before waiting:\n");
    printf("User CPU: %ld.%ld, Kernel CPU: %ld.%ld\n", rinfo.ru_utime.tv_sec,
        rinfo.ru_utime.tv_usec, rinfo.ru_stime.tv_sec, rinfo.ru_stime.tv_usec);

    if (waitpid(childPid, NULL, 0) == -1)
        errExit("waitpid");

    /* Now child is waited for */
    if (getrusage(RUSAGE_CHILDREN, &rinfo) == -1)
        errExit("getrusage");

    printf("Cpu time after waiting:\n");
    printf("User CPU: %ld.%ld, Kernel CPU: %ld.%ld\n", rinfo.ru_utime.tv_sec,
        rinfo.ru_utime.tv_usec, rinfo.ru_stime.tv_sec, rinfo.ru_stime.tv_usec);

    exit(EXIT_SUCCESS);
}

/*
    struct rusage {
        struct timeval ru_utime;
        struct timeval ru_stime;
        long   ru_maxrss;
        long   ru_ixrss;
        long   ru_idrss;
        long   ru_isrss;
        long   ru_minflt;
        long   ru_majflt;
        long   ru_nswap;
        long   ru_inblock;
        long   ru_oublock;
        long   ru_msgsnd;
        long   ru_msgrcv;
        long   ru_nsignals;
        long   ru_nvcsw;
        long   ru_nivcsw;
    };
*/
