/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 23-1 */

/* real_timer.c

   A demonstration of the use of (real-time) timers created using setitimer().

   Usage: real_timer [secs [usecs [int-secs [int-usecs]]]]
             Defaults: 2      0        0         0

   The command-line arguments are the second and microsecond settings for
   the timer's initial value and interval.

   The version of this code shown in the first print of the book contained
   an error in the main() function whereby 'maxSigs' was initialized before
   'itv', even though the initialization of the former variable depends on the
   initialization of the latter variable. See the erratum for page 983 to 984.

   Modified by Junji Tai
*/

#define _GNU_SOURCE

#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"

static volatile sig_atomic_t gotAlarm = 0;
                        /* Set nonzero on receipt of SIGALRM */

/* Retrieve and display the real time, and (if 'includeTimer' is
   TRUE) the current value and interval for the ITIMER_REAL timer */

static void displayTimes(const char *msg, int includeTimer)
{
    static struct timeval start;
    static int callNum = 0;   /* Number of calls to this function */

    if (callNum == 0)   /* Initialize elapsed time meter */
        if (gettimeofday(&start, NULL) == -1)
            errExit("gettimeofday");

    if (callNum % 20 == 0)   /* Print header every 20 lines */
        printf("       Elapsed   Value Interval\n");

    struct timeval curr;
    if (gettimeofday(&curr, NULL) == -1)
        errExit("gettimeofday");
    printf("%-7s %6.2f", msg, curr.tv_sec - start.tv_sec +
                        (curr.tv_usec - start.tv_usec) / 1000000.0);

    struct itimerval itv;
    if (includeTimer) {
        if (getitimer(ITIMER_REAL, &itv) == -1)
            errExit("gititimer");
        printf("  %6.2f  %6.2f",
                itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0,
                itv.it_interval.tv_sec + itv.it_interval.tv_usec / 1000000.0);
    }

    printf("\n");
    callNum++;
}

static void sigalrmHandler(int sig)
{
    gotAlarm = 1;
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [secs [usecs [int-secs [int-usecs]]]]\n", argv[0]);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = &sigalrmHandler;

    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    /* Set timer from the command-line arguments */

    struct itimerval itv;
    itv.it_value.tv_sec = (argc > 1) ? getNum(argv[1]) : 2;
    itv.it_value.tv_usec = (argc > 2) ? getNum(argv[2]) : 0;
    itv.it_interval.tv_sec = (argc > 3) ? getNum(argv[3]) : 0;
    itv.it_interval.tv_usec = (argc > 4) ? getNum(argv[4]) : 0;

    /* Exit after 3 signals, or on first signal if interval is 0 */

    /* Number of signals to catch before exiting */
    int maxSigs = (itv.it_interval.tv_sec == 0 &&
                itv.it_interval.tv_usec == 0) ? 1 : 3;

    displayTimes("START:", 0);
    if (setitimer(ITIMER_REAL, &itv, NULL) == -1)
        errExit("setitimer");

    clock_t prevClock = clock();
    int sigCnt = 0;   /* Number of signals so far caught */

    while (1) {

        /* Inner loop consumes at least 0.5 seconds CPU time */

        while (((clock() - prevClock) * 10 / CLOCKS_PER_SEC) < 5) {
            if (gotAlarm) {   /* Did we get a signal? */
                gotAlarm = 0;
                displayTimes("ALARM:", 1);

                sigCnt++;
                if (sigCnt >= maxSigs) {
                    printf("That's all folks\n");
                    exit(EXIT_SUCCESS);
                }
            }
        }

        prevClock = clock();
        displayTimes("Main: ", 1);
    }
}
