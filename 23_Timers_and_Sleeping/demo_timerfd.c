/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 23-8 */

/* demo_timerfd.c

   Demonstrate the use of the timerfd API, which creates timers whose
   expirations can be read via a file descriptor.

   This program is Linux-specific. The timerfd API is supported since kernel
   2.6.25. Library support is provided since glibc 2.8.

   Modified by Junji Tai 2025
*/

#include <stdlib.h>
#include <string.h>
#include <sys/timerfd.h>
#include <time.h>
#include <stdint.h>                     /* Definition of uint64_t */

#include "../lib/error_functions.h"
#include "../lib/utils.h"

void itimerspecFromStr(char *str, struct itimerspec *tsp)
{
    char *dupstr ,*cptr, *sptr;

    dupstr = strdup(str);

    cptr = strchr(dupstr, ':');
    if (cptr != NULL)
        *cptr = '\0';

    sptr = strchr(dupstr, '/');
    if (sptr != NULL)
        *sptr = '\0';

    tsp->it_value.tv_sec = atoi(dupstr);
    tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

    if (cptr == NULL) {
        tsp->it_interval.tv_sec = 0;
        tsp->it_interval.tv_nsec = 0;
    } else {
        sptr = strchr(cptr + 1, '/');
        if (sptr != NULL)
            *sptr = '\0';
        tsp->it_interval.tv_sec = atoi(cptr + 1);
        tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
    }
    free(dupstr);
}

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]] [max-exp]\n", argv[0]);

    struct itimerspec ts;
    itimerspecFromStr(argv[1], &ts);
    int maxExp = (argc > 2) ? getNum(argv[2]) : 1;

    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        errExit("timerfd_create");

    if (timerfd_settime(fd, 0, &ts, NULL) == -1)
        errExit("timerfd_settime");

    struct timespec start;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        errExit("clock_gettime");

    uint64_t numExp;
    int secs;
    int nanosecs;

    struct timespec now;

    for (int totalExp = 0; totalExp < maxExp;) {

        /* Read number of expirations on the timer, and then display
           time elapsed since timer was started, followed by number
           of expirations read and total expirations so far. */

        ssize_t s = read(fd, &numExp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            errExit("read");

        totalExp += numExp;

        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
            errExit("clock_gettime");

        secs = now.tv_sec - start.tv_sec;
        nanosecs = now.tv_nsec - start.tv_nsec;
        if (nanosecs < 0) {
            secs--;
            nanosecs += 1000000000;
        }

        printf("%d.%03d: expirations read: %llu; total=%llu\n",
                secs, (nanosecs + 500000) / 1000000,
                (unsigned long long) numExp, (unsigned long long) totalExp);
    }

    exit(EXIT_SUCCESS);
}
