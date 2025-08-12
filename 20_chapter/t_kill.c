/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 20-3 */

/* t_kill.c

   Send a signal using kill(2) and analyze the return status of the call.

   Modified by Junji Tai 2025
*/

#define _POSIX_C_SOURCE 1

#include <signal.h>
#include <sys/types.h>

#include "../lib/error_functions.h"

int main (int argc, char **argv)
{
    int s;
    int sig;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid sig-num", argv[0]);

    char *endptr;
    sig = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0')
        errExit("strtol");
    pid_t pid = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
        errExit("strtol");

    s = kill(pid, sig);

    if (sig != 0) {
        if (s == -1)
            errExit("kill");
    } else {   /* Null signal: process existence check */
        if (s == 0) {
            printf("Process exists and we can send it a signal\n");
        } else {
            if (errno == EPERM)
                printf("Process exists, but we don't have "
                       "permission to send it a signal\n");
            else if (errno == ESRCH)
                printf("Process does not exist\n");
            else
                errExit("kill");
        }
    }

    exit(EXIT_SUCCESS);
}
