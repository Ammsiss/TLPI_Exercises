/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 44-8 - Modified by Junji Tai 24-12-2025 */

/*
    This is a modified version of the listing in order to complete exersize
    52-2.
*/

#include <mqueue.h>

#include "pmsg_seqnum.h"

static char client_mq_path[CLIENT_PMSG_NAME_LEN];

/* Invoked on exit to delete client PMSG */
static void remove_pmsg(void)
{
    if (mq_unlink(client_mq_path) == -1)
        errMsg("mq_unlink");
}

int main(int argc, char** argv)
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-val]\n", argv[0]);

    snprintf(client_mq_path, CLIENT_PMSG_NAME_LEN, CLIENT_PMSG_TEMPLATE,
            (long) getpid());

    /* Create client pmsg first to avoid race condition */

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_RES_SIZE;

    mqd_t client_mqd = mq_open(client_mq_path, O_RDONLY | O_CREAT, 0660, &attr);
    if (client_mqd == (mqd_t) -1)
        errExit("mq_open - %s", client_mq_path);

    if (atexit(remove_pmsg) != 0)
        errExit("atexit");

    /* Construct request message, open server pmsg, and send message */

    struct request req;

    req.pid = getpid();
    req.seq_val = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;

    mqd_t server_mqd = mq_open(SERVER_PMSG, O_WRONLY);
    if (server_mqd == -1)
        errExit("mq_open - %s", SERVER_PMSG);

    if (mq_send(server_mqd, (char *) &req, MAX_REQ_SIZE, 0) == -1)
        errExit("mq_send");

    /* Read and display response */

    struct response resp;

    if (mq_receive(client_mqd, (char *) &resp, MAX_RES_SIZE, 0) == -1)
        errExit("mq_recieve");

    printf("Previous value: %d\n", resp.seq_num);
    exit(EXIT_SUCCESS);
}
