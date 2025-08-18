/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/*
    This is a modified version of Listing 23-3, created as a solution to
    TLPI exersise 23-2.

    Modified by Junji Tai 2025
*/

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 199309

#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

static void sigintHandler(int sig)
{
    return;   /* Just interrupt nanosleep() */
}

int main(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs nanosecs\n", argv[0]);

    struct timespec request;
    request.tv_sec = getNum(argv[1]);
    request.tv_nsec = getNum(argv[2]);

    /* Allow SIGINT handler to interrupt nanosleep() */

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = &sigintHandler;

    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    struct timeval start;
    if (gettimeofday(&start, NULL) == -1)
        errExit("gettimeofday");

    struct timespec absTime;
    if (clock_gettime(CLOCK_REALTIME, &absTime) == -1)
        errExit("clock_gettime");

    request.tv_sec += absTime.tv_sec;
    request.tv_nsec += absTime.tv_nsec;

    int s;
    struct timeval finish;
    while (1) {

        s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);
        if (s != 0 && s != EINTR)
            errExit("clock_nanosleep");

        if (gettimeofday(&finish, NULL) == -1)
            errExit("gettimeofday");
        printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec +
                        (finish.tv_usec - start.tv_usec) / 1000000.0);

        if (s == 0)
            break;   /* clock_nanosleep is finished */

        if (clock_gettime(CLOCK_REALTIME, &absTime) == -1)
            errExit("clock_gettime");
        printf("Remaining: %f\n", request.tv_sec - absTime.tv_sec +
                            (double) (request.tv_nsec - absTime.tv_nsec) / 1000000000);
    }

    printf("Sleep complete\n");
    exit(EXIT_SUCCESS);
}
