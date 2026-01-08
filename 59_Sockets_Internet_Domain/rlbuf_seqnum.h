/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 59-5 - Modified by Junji Tai 2025-01-11 */

/*
    This is a modified version of the listing in order to complete exersise
    59-1.
*/

#include "tlpi_hdr.h" // IWYU pragma: export
#include "read_line_buf.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h> // IWYU pragma: export

#define PORT_NUM "50000"        /* Port number for server */

#define INT_LEN 30              /* Size of string able to hold largest
                                   integer (including terminating '\n') */
