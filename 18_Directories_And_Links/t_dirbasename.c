/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 18-5 */

/* t_dirbasename.c

   Demonstrate the use of dirname() and basename() to break a pathname
   into directory and filename components.

   Usage: t_dirbasename path...

   The program calls dirname() and basename() for each of the pathnames
   supplied on the command-line.

   Modified by Junji Tai 2025
*/

#include <libgen.h>
#include <string.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    char *t1, *t2;
    int j;

    for (j = 1; j < argc; j++) {
        t1 = strdup(argv[j]);   /* dirname(3) and basename(3) may modify pathname */
        if (t1 == NULL)
            errExit("strdup");   /* strdup dynamically allocates */
        t2 = strdup(argv[j]);
        if (t2 == NULL)
            errExit("strdup");

        printf("%s ==> %s + %s\n", argv[j], dirname(t1), basename(t2));

        free (t1);
        free(t2);
    }

    exit(EXIT_SUCCESS);
}
