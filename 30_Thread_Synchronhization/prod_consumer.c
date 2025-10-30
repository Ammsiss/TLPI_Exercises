/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int avail = 0;

static void *threadFunc(void *arg)
{
    int interations = getNum((char *) arg, "Thread recieved invalid argument.\n");

    int s;
    for (int i = 0; i < interations; ++i) {
        sleep(1);

        /* Code to produce a unit omitted */

        if ((s = pthread_mutex_lock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_lock");

        avail++;   /* Let consumer know another unit is available */

        if ((s = pthread_mutex_unlock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_unlock");

        /* -----------------------------------*/
        if ((s = pthread_cond_signal(&cond)) != 0)   /* Wake sleeping consumer */
            errExitEN(s, "pthread_cond_signal");
        /* -----------------------------------*/
    }

    return NULL;
}

int main(int argc, char **argv)
{
    time_t t = time(NULL);

    /* Create all threads */

    int s;
    pthread_t tid;
    int totalRequired = 0;   /* Total # of units that all threads will produce */
    for (int j = 1; j < argc; ++j) {
        totalRequired += getNum(argv[j], "Input must be a integer.\n");

        if ((s = pthread_create(&tid, NULL, &threadFunc, argv[j])) != 0)
            errExitEN(s, "pthread_create");
    }

    /* Use a polling loop to check for available units */

    int numConsumed = 0;   /* Total number of units consumed so far */
    int done = 0;

    while (1) {
        if ((s = pthread_mutex_lock(&mtx)) != 0)
            errExitEN(s, "pthreaad_mutex_lock");

        /* -----------------------------------*/
         while (avail == 0)   /* Wait for something to consume */
             if ((s = pthread_cond_wait(&cond, &mtx)) != 0)
                 errExitEN(s, "pthread_cond_wait");

        /* At this point, 'mtx' is locked... */
        /* -----------------------------------*/

        while (avail > 0) {   /* Consume all available units */

            /* Do something with produced unit */

            numConsumed++;
            avail--;
            printf("T=%ld: numConsumed=%d\n", (long) (time(NULL) - t),
                    numConsumed);

            done = numConsumed >= totalRequired;
        }

        if ((s = pthread_mutex_unlock(&mtx)) != 0)
            errExitEN(s, "pthread_mutex_unlock");

        if (done)
            break;

        /* Perhaps do other work here that does not require mutex lock */
    }

    exit(EXIT_SUCCESS);
}
