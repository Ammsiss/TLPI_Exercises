/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 30-4 */

/* thread_multijoin.c

   This program creates one thread for each of its command-line arguments.
   Each thread sleeps for the number of seconds specified in the corresponding
   command-line argument, and then terminates. This sleep interval simulates
   "work" done by the thread.

   The program maintains a global array (pointed to by 'thread') recording
   information about all threads that have been created. The items of this
   array record the thread ID ('tid') and current state ('state', of type
   'enum tstate') of each thread.

   As each thread terminates, it sets its 'state' to TS_TERMINATED and
   signals the 'threadDied' condition variable. The main thread continuously
   waits on this condition variable, and is thus informed when any of the
   threads that it created has terminated. When 'numLive', which records
   the number of live threads, falls to 0, the main thread terminates.
*/

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
                /* Protects all of the following global variables */

static int totThreads = 0;      /* Total number of threads created */
static int numLive = 0;         /* Total number of threads still alive or
                                   terminated but not yet joined */
static int numUnjoined = 0;     /* Number of terminated threads that
                                   have not yet been joined */

enum tstate {                   /* Thread states */
    TS_ALIVE,                   /* Thread is alive */
    TS_TERMINATED,              /* Thread terminated, not yet joined */
    TS_JOINED                   /* Thread terminated, and joined */
};

static struct {                 /* Info about each thread */
    pthread_t tid;              /* ID of this thread */
    enum tstate state;          /* THread state (TS_* constants abgove) */
    int sleepTime;              /* Number seconds to live before terminating */
} *thread;

static void *threadFunc(void *arg)
{
    int idx = (int)(long) arg;

    sleep(thread[idx].sleepTime);   /* Simulate doing some work */
    printf("Thread %d terminating\n", idx);

    int s;
    if ((s = pthread_mutex_lock(&threadMutex)) != 0)
        errExitEN(s, "pthread_mutex_lock");

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    if ((s = pthread_mutex_unlock(&threadMutex)) != 0)
        errExitEN(s, "pthread_mutex_unlock");
    if ((s = pthread_cond_signal(&threadDied)) != 0)
        errExitEN(s, "pthread_cond_signal");

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-secs...", argv[0]);

    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL)
        errExit("calloc");

    /* Create all threads */

    int s;
    for (int i = 0; i < argc - 1; ++i) {

        thread[i].sleepTime = getNum(argv[i + 1], "Invalid Input.\n");
        thread[i].state = TS_ALIVE;

        if ((s = pthread_create(&thread[i].tid, NULL, &threadFunc, (void *)(long) i)) != 0)
            errExitEN(s, "pthread_create");
    }

    totThreads = argc - 1;
    numLive = totThreads;

    /* Join with terminated threads */

    while (numLive > 0) {
        if ((s = pthread_mutex_lock(&threadMutex)) != 0)
            errExitEN(s, "pthread_mutex_lock");

        while (numUnjoined == 0)
            if ((s == pthread_cond_wait(&threadDied, &threadMutex)) != 0)
                errExitEN(s, "pthread_cond_wait");

        for (int idx = 0; idx < totThreads; ++idx) {
            if (thread[idx].state == TS_TERMINATED) {
                if ((s = pthread_join(thread[idx].tid, NULL)) != 0)
                    errExitEN(s, "pthread_join");

                thread[idx].state = TS_JOINED;
                --numLive;
                --numUnjoined;

                printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
            }
        }

        if ((s = pthread_mutex_unlock(&threadMutex)) != 0)
            errExitEN(s, "pthread_mutex_unlock");
    }

    free(thread);
    exit(EXIT_SUCCESS);
}
