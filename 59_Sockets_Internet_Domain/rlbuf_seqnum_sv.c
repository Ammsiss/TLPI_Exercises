/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 59-6 - Modified by Junji Tai 2025-01-11 */

/*
    This is a modified version of the listing in order to complete exersise
    59-1.
*/

#define _DEFAULT_SOURCE

#include "rlbuf_seqnum.h"
#include "inet_sockets.h"

#include <netdb.h>

#define BACKLOG 50

int main(int argc, char *argv[])
{
    uint32_t seqNum;
    struct rlbuf rlbuf;
    char reqLenStr[MAX_MSG_LEN * 2];   /* Length of requested sequence */
    char seqNumStr[MAX_MSG_LEN * 2];   /* Start of granted sequence */
    struct sockaddr_storage claddr;
    int lfd, cfd, reqLen;
    socklen_t addrlen;
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [init-seq-num]\n", argv[0]);

    seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

    /* Ignore the SIGPIPE signal, so that we find out about broken connection
       errors via a failure from write(). */

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)    errExit("signal");

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try binding to */

    lfd = inetListen(PORT_NUM, BACKLOG, NULL);
    if (lfd == -1)
        errExit("inetListen");

    for (;;) {                  /* Handle clients iteratively */

        /* Accept a client connection, obtaining client's address */

        addrlen = sizeof(struct sockaddr_storage);
        cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
        if (cfd == -1) {
            errMsg("accept");
            continue;
        }

        inetAddressStr((struct sockaddr *) &claddr, addrlen, addrStr, ADDRSTRLEN);
        printf("Connection from %s\n", addrStr);

        /* Read client request, send sequence number back */

        read_line_buf_init(cfd, &rlbuf);

        if (read_line_buf(&rlbuf, reqLenStr) <= 0) {
            close(cfd);
            continue;
        }

        reqLen = atoi(reqLenStr);
        if (reqLen <= 0) {              /* Watch for misbehaving clients */
            close(cfd);
            continue;                   /* Bad request; skip it */
        }

        snprintf(seqNumStr, MAX_MSG_LEN * 2, "%d\n", seqNum);
        if (write(cfd, seqNumStr, strlen(seqNumStr)) != (int) strlen(seqNumStr))
            fprintf(stderr, "Error on write");

        seqNum += reqLen;               /* Update sequence number */

        if (close(cfd) == -1)           /* Close connection */
            errMsg("close");
    }
}
