/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
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

#include "tlpi_hdr.h" // IWYU pragma: export

#include <time.h>
#include <sys/select.h>

int main(int argc, char **argv)
{
    int numPipes, randPipe, numWrites, j;
    int (*pfds)[2];

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-pipes [num-writes]", argv[0]);

    numPipes = getInt(argv[1], GN_GT_0, "num-pipes");
    numWrites = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-writes") : 1;

    pfds = calloc(numPipes, sizeof(int [2]));
    if (pfds == NULL)
        errExit("calloc");

    int nfds = 0;
    fd_set read_set;
    FD_ZERO(&read_set);

    for (j = 0; j < numPipes; j++) {
        if (pipe(pfds[j]) == -1)
            errExit("pipe %d", j);
        if (pfds[j][0] > FD_SETSIZE)
            fatal("Pipe fd value exceeds FD_SETSIZE");
        if (pfds[j][0] >= nfds)
            nfds = pfds[j][0] + 1;
        FD_SET(pfds[j][0], &read_set);
    }

    srandom((int) time(NULL));
    for (j = 0; j < numWrites; j++) {
        randPipe = random() % numPipes;
        printf("Writing to fd: %3d (read fd: %3d)\n",
                pfds[randPipe][1], pfds[randPipe][0]);
        if (write(pfds[randPipe][1], "a", 1) == -1)
            errExit("write %d", pfds[randPipe][1]);
    }

    struct timeval tv = { /*tv_sec=*/0, /*tv_usec=*/0 };
    int ready = select(nfds, &read_set, NULL, NULL, &tv);
    if (ready == -1)
        errExit("select");

    printf("ready = %d\n", ready);

    if (ready > 0)
        for (j = 0; j < numPipes; ++j)
            if (FD_ISSET(pfds[j][0], &read_set))
                printf("Readable: %3d\n", pfds[j][0]);

    free(pfds);
    exit(EXIT_SUCCESS);
}
