/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2025.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 60-4 - Modified by Junji Tai 01/18/26 - 13:37 */

/*
    This is a modified version of the listing in order to complete exersise
    60-1 and 60-2.
*/

#include "tlpi_hdr.h" // IWYU pragma: export

#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>
#include "become_daemon.h"
#include "inet_sockets.h"

#define SERVICE "echo"
#define BUF_SIZE 4096

static volatile sig_atomic_t sigchild_caught = 0;
static int child_count = 0;

static void grimReaper(int sig)
{
    (void) sig;

    sigchild_caught = 1;
}

static void handleRequest(int cfd)
{
    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
        if (write(cfd, buf, numRead) != numRead) {
            syslog(LOG_ERR, "write() failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        syslog(LOG_ERR, "Error from read(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sleep(10);
}

int main(int argc, char **argv)
{
    /* Should be all it takes to work with inetd */
    if (argc == 2 && strcmp(argv[1], "-i") == 0)
        handleRequest(STDIN_FILENO);

    int lfd, cfd;
    struct sigaction sa;

    if (becomeDaemon(0) == -1)
        errExit("becomeDaemon");

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        syslog(LOG_ERR, "Error from sigaction(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    lfd = inetListen(SERVICE, 10, NULL);
    if (lfd == -1) {
        syslog(LOG_ERR, "Could not create server socket (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (;;) {
        /* Move waitpid culling outside of signal handler because we need
         * to decrement child_count which is not async signal safe */
        if (sigchild_caught) {
            sigchild_caught = 0;
            while (waitpid(-1, NULL, WNOHANG) > 0)
                if (child_count > 0)
                    --child_count;
        }

        cfd = accept(lfd, NULL, NULL);
        if (cfd == -1) {
            if (errno == EINTR) {
                syslog(LOG_INFO, "Interupted by SIGCHLD");
                continue;
            }

            syslog(LOG_ERR, "Failure in accept(): %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (child_count <= 2) {
            switch (fork()) {
            case -1:
                syslog(LOG_ERR, "Can't create child (%s)", strerror(errno));
                close(cfd);
                break;

            case 0:
                close(lfd);
                handleRequest(cfd);
                _exit(EXIT_SUCCESS);

            default:
                close(cfd);
                ++child_count;
                break;
            }
        } else {
            syslog(LOG_WARNING, "Max child_count reached: %d", child_count);
        }
    }
}
