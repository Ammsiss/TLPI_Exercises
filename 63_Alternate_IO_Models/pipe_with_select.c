/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 63-2 */

/* poll_pipes.c - Modified by Junji Tai 01/21/26 - 17:03

   This is a modified version of the listing to complete
   exersize 63-1.
*/

#define _GNU_SOURCE

#include <time.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/select.h>

#include "tlpi_hdr.h" // IWYU pragma: export

int main(int argc, char *argv[]) {
    int numPipes, randPipe, numWrites, j;
    int (*pfds)[2];                     /* File descriptors for all pipes */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-pipes [num-writes]", argv[0]);

    /* Allocate the arrays that we use. The arrays are sized according
       to the number of pipes specified on command line */

    numPipes = getInt(argv[1], GN_GT_0, "num-pipes");
    numWrites = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-writes") : 1;

    pfds = calloc(numPipes, sizeof(int [2]));
    if (pfds == NULL)
        errExit("calloc");

    /* Create the number of pipes specified on command line */

    int nfds = 0;
    fd_set read_set;
    FD_ZERO(&read_set);

    for (j = 0; j < numPipes; j++) {
        if (pipe(pfds[j]) == -1)
            errExit("pipe %d", j);

        /* Store the value 1 larger then the largest fd */

        if (pfds[j][0] > FD_SETSIZE)
            fatal("Pipe fd value exceeds FD_SETSIZE");

        if (pfds[j][0] >= nfds)
            nfds = pfds[j][0] + 1;

        /* Add read end to the fd_set */

         FD_SET(pfds[j][0], &read_set);
    }

    /* Perform specified number of writes to random pipes */

    srandom((int) time(NULL));
    for (j = 0; j < numWrites; j++) {
        randPipe = random() % numPipes;
        printf("Writing to fd: %3d (read fd: %3d)\n",
                pfds[randPipe][1], pfds[randPipe][0]);
        if (write(pfds[randPipe][1], "a", 1) == -1)
            errExit("write %d", pfds[randPipe][1]);
    }

    /* Check which pipes have data available for reading */

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int num_ready = select(nfds, &read_set, NULL, NULL, &tv);
    if (num_ready == -1)
        errExit("select");

    printf("Select returned %d\n", num_ready);

    if (num_ready > 0)
        for (j = 0; j < numPipes; ++j)
            if (FD_ISSET(pfds[j][0], &read_set))
                printf("Readable: %3d\n", pfds[j][0]);

    free(pfds);
    exit(EXIT_SUCCESS);
}
