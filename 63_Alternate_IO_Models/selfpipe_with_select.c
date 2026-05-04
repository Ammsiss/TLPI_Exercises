/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 63-9 */

/*  self_pipe.c

   Employ the self-pipe trick so that we can avoid race conditions while both
   selecting on a set of file descriptors and also waiting for a signal.

   Usage as shown in synopsis below; for example:

        self_pipe - 0
*/

#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/time.h>
#if ! defined(__hpux)   /* HP-UX 11 doesn't have this header file */
#include <sys/select.h>
#endif
#include <fcntl.h>
#include <signal.h>
#include <sys/param.h>
#include <poll.h>

static int pfd[2];                      /* File descriptors for pipe */

static void
handler(int sig)
{
    (void) sig;

    int savedErrno;                     /* In case we change 'errno' */

    savedErrno = errno;
    if (write(pfd[1], "x", 1) == -1 && errno != EAGAIN)
        errExit("write");
    errno = savedErrno;
}

int
main(int argc, char *argv[])
{
    struct pollfd *fds;
    int ready, flags;
    struct sigaction sa;
    int fd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s fd...", argv[0]);

    /* Initialize pollfd array and nfds for poll() */

    int nfds = argc;   /* -1 because of first cli arg but +1 for pipe */
    fds = calloc(nfds, sizeof(struct pollfd));

    for (int i = 1; i < argc; i++) {
        fd = getInt(argv[i], 0, "fd");
        fds[i - 1].fd = fd;
        fds[i - 1].events = POLLIN;
    }

    /* Create pipe before establishing signal handler to prevent race */

    if (pipe(pfd) == -1)
        errExit("pipe");

    fds[nfds - 1].fd = pfd[0];
    fds[nfds - 1].events = POLLIN;

    /* Make read and write ends of pipe nonblocking */

    flags = fcntl(pfd[0], F_GETFL);
    if (flags == -1)
        errExit("fcntl-F_GETFL");
    flags |= O_NONBLOCK;                /* Make read end nonblocking */
    if (fcntl(pfd[0], F_SETFL, flags) == -1)
        errExit("fcntl-F_SETFL");

    flags = fcntl(pfd[1], F_GETFL);
    if (flags == -1)
        errExit("fcntl-F_GETFL");
    flags |= O_NONBLOCK;                /* Make write end nonblocking */
    if (fcntl(pfd[1], F_SETFL, flags) == -1)
        errExit("fcntl-F_SETFL");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;           /* Restart interrupted reads()s */
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    while ((ready = poll(fds, nfds, -1)) == -1 && errno == EINTR)
        continue;
    if (ready == -1)
        errExit("poll");

    if (fds[nfds - 1].revents & POLLIN) {   /* Handler was called */
        printf("A signal was caught\n");

        while (1) {
            char c;
            if (read(pfd[0], &c, 1) == -1) {
                if (errno == EAGAIN)
                    break;
                else
                    errExit("read");
            }
        }

        /* Perform any actions that should be taken in response to signal */
    }

    printf("ready = %d\n", ready);
    for (int i = 0; i < nfds - 1; ++i) {
        if (fds[i].revents & POLLIN)
            printf("%d: %s\n", fds[i].fd, "r");
        else
            printf("%d:\n", fds[i].fd);
    }

    /* And check if read end of pipe is ready */

    printf("%d: %s   (read end of pipe)\n", fds[nfds - 1].fd,
            (fds[nfds - 1].revents & POLLIN) ? "r" : "");

    exit(EXIT_SUCCESS);
}
