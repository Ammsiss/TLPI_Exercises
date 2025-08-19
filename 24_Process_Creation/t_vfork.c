/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 24-4 */

/* t_vfork.c

   Demonstrate the use of vfork() to create a child process.

   Modified by Junji Tai 2025

   See t_vfork_diff for changes
*/

#define _BSD_SOURCE   /* To get vfork() declaration from <unistd.h>
                         in case _XOPEN_SOURCE >= 700 */
#include <unistd.h>

#include "../lib/error_functions.h"

int main(void)
{
    int istack = 222;

    switch (vfork()) {
    case -1:
        errExit("vfork");
    case 0:   /* Child executes first, in parent's memory space */
        sleep(3);   /* Even if we sleep for a while, parent is not scheduled */
        write(STDOUT_FILENO, "Child executing\n", 16);
        istack *= 3;   /* This change will be seen by parent */
        _exit(EXIT_SUCCESS);
    default:
        write(STDOUT_FILENO, "Parent executing\n", 17);
        printf("istack=%d\n", istack);
        exit(EXIT_SUCCESS);
    }
}
