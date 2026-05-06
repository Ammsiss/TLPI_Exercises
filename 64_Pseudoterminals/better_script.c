/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 64-3 - Modified by Junji Tai 01/18/26 - 13:37

    This is a modified version of the listing in order to complete exersise
    64-2 and 64-3
*/

#include "tlpi_hdr.h" // IWYU pragma: export

#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <termios.h>
#if ! defined(__hpux)
/* HP-UX 11 doesn't have this header file */
#include <sys/select.h>
#endif
#include "pty_fork.h"           /* Declaration of ptyFork() */
#include "tty_functions.h"      /* Declaration of ttySetRaw() */

#define BUF_SIZE 256
#define MAX_SNAME 1000

int scriptFd, tscriptFd;
struct termios ttyOrig;

static void             /* Reset terminal mode on program exit */
ttyReset(void)
{
    time_t t = time(NULL);
    char *time_str = ctime(&t);

    if (write(scriptFd, time_str, 25) == -1)
        errExit("write");

    if (tcsetattr(STDIN_FILENO, TCSANOW, &ttyOrig) == -1)
        errExit("tcsetattr");
}

static volatile sig_atomic_t sigwinch_caught = 0;

void sigwinchHandler(int sig)
{
    (void) sig;

    sigwinch_caught = 1;
}

int
main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigwinchHandler;

    if (sigaction(SIGWINCH, &sa, NULL) == -1)
        errExit("sigaction");

    char slaveName[MAX_SNAME];
    char timestamp[100];
    char *shell;
    int masterFd;
    struct winsize ws;
    fd_set inFds;
    char buf[BUF_SIZE];
    ssize_t numRead;
    pid_t childPid;

    /* Retrieve the attributes of terminal on which we are started */

    if (tcgetattr(STDIN_FILENO, &ttyOrig) == -1)
        errExit("tcgetattr");
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
        errExit("ioctl-TIOCGWINSZ");

    /* Create a child process, with parent and child connected via a
       pty pair. The child is connected to the pty slave and its terminal
       attributes are set to be the same as those retrieved above. */

    childPid = ptyFork(&masterFd, slaveName, MAX_SNAME, &ttyOrig, &ws);
    if (childPid == -1)
        errExit("ptyFork");

    if (childPid == 0) {        /* Child: execute a shell on pty slave */

        /* If the SHELL variable is set, use its value to determine
           the shell execed in child. Otherwise use /bin/sh. */

        shell = getenv("SHELL");
        if (shell == NULL || *shell == '\0')
            shell = "/bin/sh";

        execlp(shell, shell, (char *) NULL);
        errExit("execlp");      /* If we get here, something went wrong */
    }

    /* Parent: relay data between terminal and pty master */

    scriptFd = open((argc > 1) ? argv[1] : "typescript",
                        O_WRONLY | O_CREAT | O_TRUNC,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                                S_IROTH | S_IWOTH);
    if (scriptFd == -1)
        errExit("open typescript");

    tscriptFd = open("typescript.timed", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (tscriptFd == -1)
        errExit("open");

    /* Place terminal in raw mode so that we can pass all terminal
     input to the pseudoterminal master untouched */

    ttySetRaw(STDIN_FILENO, &ttyOrig);

    if (atexit(ttyReset) != 0)
        errExit("atexit");

    /* Loop monitoring terminal and pty master for input. If the
       terminal is ready for input, then read some bytes and write
       them to the pty master. If the pty master is ready for input,
       then read some bytes and write them to the terminal. */

    time_t t = time(NULL);
    char *time_str = ctime(&t);

    if (write(scriptFd, time_str, 25) == -1)
        errExit("write");

    struct timespec start_ts;
    if (clock_gettime(CLOCK_MONOTONIC, &start_ts) == -1)
        errExit("clock_gettime");

    struct timespec ts;

    for (;;) {
        if (sigwinch_caught) {
            if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
                errExit("ioctl-TIOCGWINSZ");
            if (ioctl(masterFd, TIOCSWINSZ, &ws) < 0)
                errExit("ioctl-TIOCSWINSZ");
            sigwinch_caught = 0;
        }

        FD_ZERO(&inFds);
        FD_SET(STDIN_FILENO, &inFds);
        FD_SET(masterFd, &inFds);

        if (select(masterFd + 1, &inFds, NULL, NULL, NULL) == -1) {
            if (errno == EINTR)
                continue;
            else
                errExit("select");
        }

        if (FD_ISSET(STDIN_FILENO, &inFds)) {   /* stdin --> pty */
            numRead = read(STDIN_FILENO, buf, BUF_SIZE);
            if (numRead <= 0)
                exit(EXIT_SUCCESS);

            if (write(masterFd, buf, numRead) != numRead)
                fatal("partial/failed write (masterFd)");
        }

        if (FD_ISSET(masterFd, &inFds)) {      /* pty --> stdout+file */
            numRead = read(masterFd, buf, BUF_SIZE);
            if (numRead <= 0)
                exit(EXIT_SUCCESS);

            if (write(STDOUT_FILENO, buf, numRead) != numRead)
                fatal("partial/failed write (STDOUT_FILENO)");

            /* Write to timestamp file */

            if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
                errExit("clock_gettime");

            ts.tv_sec = ts.tv_sec - start_ts.tv_sec;
            ts.tv_nsec = ts.tv_nsec - start_ts.tv_nsec;

            int ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

            snprintf(timestamp, 100, "%d ", ms);

            if (write(tscriptFd, timestamp, strlen(timestamp)) !=
                    (ssize_t) strlen(timestamp))
                fatal("partial/failed write (tscriptFd)");

            for (int i = 0; i < numRead; ++i) {
                if (buf[i] == '\n') {
                    if (write(tscriptFd, "\\n", 2) != 2)
                        fatal("partial/failed write (tscriptFd)");
                } else {
                    if (write(tscriptFd, &buf[i], 1) != 1)
                        fatal("partial/failed write (tscriptFd)");
                }
            }

            if (write(tscriptFd, "\n", 1) != 1)
                fatal("partial/failed write (tscriptFd)");

            /* Write to regular script file */

            if (write(scriptFd, buf, numRead) != numRead)
                fatal("partial/failed write (scriptFd)");
        }
    }
}
