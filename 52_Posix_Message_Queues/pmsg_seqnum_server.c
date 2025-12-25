/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 44-7 - Modified by Junji Tai 24-12-2025 */

/*
    This is a modified version of the listing in order to complete exersize
    52-2.
*/

#define _GNU_SOURCE

#include <signal.h>
#include <mqueue.h>
#include <signal.h>

#include "pmsg_seqnum.h"
#include "tlpi_hdr.h" // IWYU pragma: export

static volatile sig_atomic_t quit = 0;

void sigCatcher(int sig)
{
    (void) sig;

    quit = 1;
}

int main(void)
{
    int seq_val = 0;

    /* Catch SIGINT and SIGTERM so we can unlink our pmsg */

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigCatcher;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGTERM, &sa, NULL) == -1)
        errExit("sigaction");

    /* Create server pmsg, and open it for reading */

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_REQ_SIZE;

    int flags = O_RDONLY | O_CREAT | O_EXCL;
    mqd_t server_mqd = mq_open(SERVER_PMSG, flags, 0600, &attr);
    if (server_mqd == (mqd_t) -1)
        errExit("mq_open - %s", SERVER_PMSG);

    struct request req;
    char client_pmsg_path[CLIENT_PMSG_NAME_LEN];

    while (1) {
        if (mq_receive(server_mqd, (char *) &req, MAX_REQ_SIZE, NULL) == -1) {
            errMsg("mq_receive - %s", SERVER_PMSG);
            if (quit)   /* Normally don't print error on handled signal but */
                break;  /* just to see its working I'll do it. */
        }

        /* Open client pmsg */

        snprintf(client_pmsg_path, CLIENT_PMSG_NAME_LEN, CLIENT_PMSG_TEMPLATE,
                (long) req.pid);

        mqd_t client_mqd = mq_open(client_pmsg_path, O_WRONLY);
        if (client_mqd == (mqd_t) -1) {
            errMsg("Error opening client pmsg; dropping request");
            continue;
        }

        /* Send response and close FIFO */

        struct response resp = { seq_val };

        if (mq_send(client_mqd, (char *) &resp, MAX_RES_SIZE, 0) == -1) {
            errMsg("mq_send - %s", client_pmsg_path);
            if (quit)
                break;
        }
        if (mq_close(client_mqd) == -1)
            errMsg("mq_close - %s", client_pmsg_path);

        seq_val += req.seq_val;  /* Update our sequence number */
    }

    if (mq_unlink(SERVER_PMSG) == -1)
        errMsg("mq_unlink - %s", SERVER_PMSG);
    exit(EXIT_SUCCESS);
}
