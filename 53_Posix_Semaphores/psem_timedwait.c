/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 44-7 - Modified by Junji Tai 26-12-2025 */

/*
    This is a modified version of the listing in order to complete exersise
    53-2.
*/

#include "tlpi_hdr.h" // IWYU pragma: export

#include <semaphore.h>
#include <time.h>

int main(int argc, char **argv)
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sem-name timeout\n", argv[0]);

    sem_t *sem = sem_open(argv[1], 0);
    if (sem == SEM_FAILED)
        errExit("sem_open");

    int timeout = getInt(argv[2], GN_GT_0, "timeout");

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        errExit("clock_gettime");

    ts.tv_sec += timeout;

    if (sem_timedwait(sem, &ts) == -1)
        errExit("sem_wait");   /* Let errno printing inform of a timeout */

    printf("%ld sem_wait() succeeded\n", (long) getpid());
    exit(EXIT_SUCCESS);
}
