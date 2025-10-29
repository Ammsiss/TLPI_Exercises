/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 22-7 */

/* signalfd_sigval.c

   Usage: signalfd_sigval sig-num...

   Demonstrate the use of signalfd() to receive signals via a file descriptor.

   This program is Linux-specific. The signalfd API is supported since kernel
   2.6.22.

   Modified by Junji Tai
*/

#define _GNU_SOURCE

#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

int main (int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sig-num...\n", argv[0]);

    printf("%s: PID = %ld\n", argv[0], (long) getpid());

    sigset_t mask;
    sigemptyset(&mask);
    for (int j = 1; j < argc; j++)
        sigaddset(&mask, getNum(argv[j]));

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        errExit("sigprocmask");

    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1)
        errExit("signalfd");

    while (1) {
        struct signalfd_siginfo fdsi;
        ssize_t s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo))
            errExit("read");

        printf("%s: got signal %d", argv[0], fdsi.ssi_signo);
        if (fdsi.ssi_code == SI_QUEUE) {
            printf("; ssi_pid = %d; ", fdsi.ssi_pid);
            printf("ssi_int = %d", fdsi.ssi_int);
        }
        printf("\n");
    }
}
