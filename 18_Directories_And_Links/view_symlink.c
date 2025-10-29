/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 18-4 */

/* view_symlink.c

   Demonstrate the use of readlink() and realpath() to read and display
   the contents of a symbolic link.

   Modified by Junji Tai 2025
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>             /* For definition of PATH_MAX */
#include <fcntl.h>            /* Definition of AT_* constants */
#include <unistd.h>

#include "../lib/error_functions.h"

#define BUF_SIZE PATH_MAX

int main(int argc, char **argv)
{
    char buf[BUF_SIZE];
    ssize_t numBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    /* Check if readlink() failed with EINVAL to determine if
       the supplied pathname is not a sym link. Alternatively we
       could have used lstat() */

    numBytes = readlink(argv[1], buf, BUF_SIZE - 1);
    if (numBytes ==-1){
        if (errno == EINVAL) {
            fprintf(stderr, "Not symbolic link.\n");
            exit(EXIT_FAILURE);
        } else
            errExit("readlink");
    }
    buf[numBytes] = '\0';
    printf("readlink: %s --> %s\n", argv[1], buf);

    if (realpath(argv[1], buf) == NULL)
        errExit("realpath");
    printf("realpath: %s --> %s\n", argv[1], buf);

    exit(EXIT_SUCCESS);
}
