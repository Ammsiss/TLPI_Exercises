/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 59-7 - Modified by Junji Tai 2025-01-11 */

/*
    This is a modified version of the listing in order to complete exersise
    59-1.
*/

#include "rlbuf_seqnum.h"
#include "inet_sockets.h"

#include <netdb.h>

int main(int argc, char *argv[])
{
    struct rlbuf rlbuf;
    char *reqLenStr;                    /* Requested length of sequence */
    char seqNumStr[MAX_MSG_LEN];            /* Start of granted sequence */
    int cfd;
    ssize_t numRead;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-host [sequence-len]\n", argv[0]);

    /* Call getaddrinfo() to obtain a list of addresses that
       we can try connecting to */

    cfd = inetConnect(NULL, PORT_NUM, SOCK_STREAM);
    if (cfd == -1)
        errExit("inetConnect");

    /* Send requested sequence length, with terminating newline */

    reqLenStr = (argc > 2) ? argv[2] : "1";
    if (write(cfd, reqLenStr, strlen(reqLenStr)) != (int) strlen(reqLenStr))
        fatal("Partial/failed write (reqLenStr)");
    if (write(cfd, "\n", 1) != 1)
        fatal("Partial/failed write (newline)");

    /* Read and display sequence number returned by server */

    read_line_buf_init(cfd, &rlbuf);

    numRead = read_line_buf(&rlbuf, seqNumStr);
    if (numRead == -1)
        errExit("readLine");
    if (numRead == 0)
        fatal("Unexpected EOF from server");

    printf("Sequence number: %s", seqNumStr);   /* Includes '\n' */

    exit(EXIT_SUCCESS);                         /* Closes 'cfd' */
}
