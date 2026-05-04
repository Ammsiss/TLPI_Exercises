/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 63-3 */

/* demo_sigio.c

   A trivial example of the use of signal-driven I/O.
*/

#define _GNU_SOURCE

#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include "tty_functions.h"      /* Declaration of ttySetCbreak() */
#include "tlpi_hdr.h"

static volatile sig_atomic_t gotSigio = 0;
                                /* Set nonzero on receipt of SIGIO */

static void siginfo_handler(int sig, siginfo_t *siginfo, void *ucontext)
{
    (void) sig;
    (void) ucontext;

    gotSigio = 1;

    /* Not async signal safe! */

    printf("si_fd = %d   si_code = ", siginfo->si_fd);

    switch(siginfo->si_code) {
    case POLL_IN:  printf("POLL_IN");  break;
    case POLL_OUT: printf("POLL_OUT"); break;
    case POLL_MSG: printf("POLL_MSG"); break;
    case POLL_ERR: printf("POLL_ERR"); break;
    case POLL_PRI: printf("POLL_PRI"); break;
    case POLL_HUP: printf("POLL_HUP"); break;
    }
    printf("\n");
}

int main(void)
{
    int flags, j, cnt;
    struct termios origTermios;
    char ch;
    struct sigaction sa;
    int done;

    /* Establish handler for "I/O possible" signal */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sa.sa_sigaction = siginfo_handler;
    if (sigaction(SIGRTMIN + 5, &sa, NULL) == -1)
        errExit("sigaction");

    /* Set owner process that is to receive "I/O possible" signal */

    if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1)
        errExit("fcntl(F_SETOWN)");

    if (fcntl(STDIN_FILENO, F_SETSIG, SIGRTMIN + 5) == -1)
        errExit("fcntl(F_SETSIG)");

    /* Enable "I/O possible" signaling and make I/O nonblocking
       for file descriptor */

    flags = fcntl(STDIN_FILENO, F_GETFL);
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1)
        errExit("fcntl(F_SETFL)");

    /* Place terminal in cbreak mode */

    if (ttySetCbreak(STDIN_FILENO, &origTermios) == -1)
        errExit("ttySetCbreak");

    for (done = 0, cnt = 0; !done ; cnt++) {
        for (j = 0; j < 100000000; j++)
            continue;                   /* Slow main loop down a little */

        if (gotSigio) {                 /* Is input available? */
            gotSigio = 0;

            /* Read all available input until error (probably EAGAIN)
               or EOF (not actually possible in cbreak mode) or a
               hash (#) character is read */

            while (read(STDIN_FILENO, &ch, 1) > 0 && !done) {
                printf("cnt=%d; read %c\n", cnt, ch);
                done = ch == '#';
            }
        }
    }

    /* Restore original terminal settings */

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios) == -1)
        errExit("tcsetattr");
    exit(EXIT_SUCCESS);
}
