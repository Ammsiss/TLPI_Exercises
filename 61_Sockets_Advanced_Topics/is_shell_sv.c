#include "tlpi_hdr.h" // IWYU pragma: export
#include "is_shell.h"
#include "inet_sockets.h"
#include "dynamic_array.h"

#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

static volatile sig_atomic_t sigint_caught = 0;

void sigint_handler(int sig)
{
    (void) sig; /* Just catch SIGINT so we can exit gracefully */

    sigint_caught = 1;
}

void sigchild_handler(int sig)
{
    (void) sig;

    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;

    errno = saved_errno;
}

void handle_request(int cfd)
{
    DynArray da;
    if (da_init(&da, 0, sizeof(char)) == -1)
        fatal("da_init");

    char c;
    int num_read;
    while (1) {
        if ((num_read = read(cfd, &c, 1)) == -1)
            errExit("child: read");

        if (num_read == 0)
            break;

        char *element = da_push(&da);
        if (element == NULL)
            fatal("da_push");
        *element = c;
    }

    *((char *) da_push(&da)) = '\0';

    if (dup2(cfd, STDOUT_FILENO) == -1)
        errExit("dup2");
    if (dup2(cfd, STDERR_FILENO) == -1)
        errExit("dup2");

    if (close(cfd) == -1)
        errExit("close");

    int wstatus;
    switch (wstatus = system((char *) da.data)) {
    case -1:
        errExit("system");
    default:
        break;
    }

    da_free(&da);
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    sa.sa_handler = sigchild_handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    int sfd = inetListen(PORT, 10, NULL);
    if (sfd == -1)
        errExit("inetListen");

    struct sockaddr_storage claddr;
    socklen_t socklen;

    while (1) {
        socklen = sizeof(struct sockaddr_storage);

        int cfd = accept(sfd, (struct sockaddr *) &claddr, &socklen);
        if (cfd == -1) {
            if (errno == EINTR) {
                if (sigint_caught) {
                    printf("\nSIGINT caught. exiting\n");
                    break;
                }
                continue;
            }

            fprintf(stderr, "Error accepting client; dropping request\n");
            continue;
        }

        switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
            close(sfd);   /* Child doesn't need listening socket */
            handle_request(cfd);

            /* This _exit() sends EOF to client so they know output
             * is done. This works becuase stdout and stderr of
             * the exected shell pointed to the socket so by proxy
             * of the shell exiting all output has to have already
             * been written to the socket */

            _exit(EXIT_SUCCESS);
        default:
            close(cfd); /* Parent doesn't need this anymore */
            break;
        }
    }
}
