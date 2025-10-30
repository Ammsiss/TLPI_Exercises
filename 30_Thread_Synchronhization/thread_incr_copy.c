/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 30-1 */

/* thread_incr.c

   This program employs two POSIX threads that increment the same global
   variable, without using any synchronization method. As a consequence,
   updates are sometimes lost.

   See also thread_incr_mutex.c.
*/
#include <pthread.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

struct Payload {
    int loops;
    int id;
};

static volatile int global = 0;   /* "volatile" prevents compiler optimizations
                                   of arithmetic operations on 'glob' */
static void *                   /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg)
{
    struct Payload *data = arg;

    int local;
    for (int j = 0; j < data->loops; j++) {
        local = global;
        local++;
        global = local;

        printf("id = %d, global = %d\n", data->id, global);
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? getNum(argv[1], NULL) : 1000000;

    struct Payload p1 = { loops, 1 };
    struct Payload p2 = { loops, 2 };

    /* Create threads */
    s = pthread_create(&t1, NULL, threadFunc, &p1);
    if (s != 0) errExitEN(s, "pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &p2);
    if (s != 0) errExitEN(s, "pthread_create");

    /* Join threads */
    s = pthread_join(t1, NULL);
    if (s != 0) errExitEN(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0) errExitEN(s, "pthread_join");

    printf("glob = %d\n", global);
    exit(EXIT_SUCCESS);
}
