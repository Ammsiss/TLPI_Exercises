/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 52-5 - Modified by Junji Tai*/

/* pmsg_receive.c

   This is a modified version of the program in order to complete
   exersize 52-1
*/

#include "tlpi_hdr.h" /* IWYU pragma: export */

#include <mqueue.h>
#include <time.h>
#include <fcntl.h>


static void usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-n] [-t sec] mq-name\n", progName);
    fprintf(stderr, "    -n           Use O_NONBLOCK flag\n");
    fprintf(stderr, "    -t           "
            "Specify timeout for mq_timedreceive()\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int flags, opt;
    mqd_t mqd;
    unsigned int prio;
    void *buffer;
    struct mq_attr attr;
    struct timespec ts;
    ssize_t numRead;

    int timeout = 0;

    flags = O_RDONLY;
    while ((opt = getopt(argc, argv, "nt:")) != -1) {
        switch (opt) {
        case 'n':
            flags |= O_NONBLOCK;
            break;
        case 't':
            timeout = getInt(optarg, GN_NONNEG, "timeout");
            break;
        default:
            usageError(argv[0]);
        }
    }

    if (optind >= argc)
        usageError(argv[0]);

    mqd = mq_open(argv[optind], flags);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");

    if (mq_getattr(mqd, &attr) == -1)
        errExit("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        errExit("malloc");

    /* Get realtime and add 'timeout' to it for a relative timeout */

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        errExit("clock_gettime");
    ts.tv_sec += timeout;

    numRead = mq_timedreceive(mqd, buffer, attr.mq_msgsize, &prio, &ts);
    if (numRead == -1)   /* On ETIMEDOUT print errno like normal */
        errExit("mq_receive");

    printf("Read %ld bytes; priority = %u\n", (long) numRead, prio);
    if (write(STDOUT_FILENO, buffer, numRead) == -1)
        errExit("write");
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}
