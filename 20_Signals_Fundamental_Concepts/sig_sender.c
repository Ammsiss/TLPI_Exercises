/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 20-6 */

/* sig_sender.c

   Usage: sig_sender PID num-sigs sig [sig2]

   Send signals to sig_receiver.c.

   Sends 'num-sigs' signals of type 'sig' to the process with the specified PID.
   If a fourth command-line argument is supplied, send one instance of that
   signal, after sending the previous signals.

   Modified by Junji Tai
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);

    char *end;
    int pid = strtol(argv[1], &end, 10);
    if (*end != '\0')
        errExit("strtol");
    int numSigs = strtol(argv[2], &end, 10);
    if (*end != '\0')
        errExit("strtol");
    int sig = strtol(argv[3], &end, 10);
    if (*end != '\0')
        errExit("strtol");

    /* Send signals to receiver */

    printf("%s: sending signal %d to process %ld %d times\n",
            argv[0], sig, (long) pid, numSigs);

    for (int j = 0; j < numSigs; j++)
        if (kill(pid, sig) == -1)
            errExit("kill");

    /* If a fourth command-line argument was specified, sned that signal */

    if (argc > 4) {
        sig = strtol(argv[4], &end, 10);
        if (*end != '\0')
            errExit("strtol");
        if (kill(pid, sig) == -1)
            errExit("kill");
    }

    printf("%s: exiting\n", argv[0]);
    exit(EXIT_SUCCESS);
}
