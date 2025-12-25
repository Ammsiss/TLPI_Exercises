/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 44-6 - Modified by Junji Tai 24-12-2025 */

/*
    This is a modified version of the listing in order to complete exersize
    52-2.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tlpi_hdr.h" // IWYU pragma: export

#define SERVER_PMSG "/seqnum_pmsg"

#define CLIENT_PMSG_TEMPLATE "/seqnum_cl.%ld"
#define CLIENT_PMSG_NAME_LEN (sizeof(CLIENT_PMSG_TEMPLATE) + 20)

#define MAX_REQ_SIZE (sizeof(struct request))
#define MAX_RES_SIZE (sizeof(struct response))

struct request {
    pid_t pid;
    int seq_val;
};

struct response {
    int seq_num;
};
