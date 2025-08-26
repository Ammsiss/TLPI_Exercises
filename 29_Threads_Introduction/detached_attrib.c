/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 29-2 */

/* detached_attrib.c

   An example of the use of POSIX thread attributes (pthread_attr_t):
   creating a detached thread.
*/

#include <pthread.h>

#include "../lib/error_functions.h"

static void* threadFunc(void *x)
{
    return x;
}

int main(int argc, char **argv)
{
    pthread_attr_t attr;

    int s;
    if (pthread_attr_init(&attr) != 0)
        errExitEN(s, "pthread_attr_init");

    if ((s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0)
        errExitEN(s, "pthread_attr_setdetachstate");

    pthread_t thr;
    if ((s = pthread_create(&thr, &attr, &threadFunc, (void *) 1)) != 0)
        errExitEN(s, "pthread_create");

    if ((s = pthread_attr_destroy(&attr)) != 0)   /* no longer needed */
        errExitEN(s, "pthread_attr_destroy");

    if ((s = pthread_join(thr, NULL)) != 0)
        errExitEN(s, "pthread_join failed as expected");

    exit(EXIT_SUCCESS);
}
