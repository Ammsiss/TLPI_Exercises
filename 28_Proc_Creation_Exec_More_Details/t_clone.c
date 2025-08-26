#define _GNU_SOURCE

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <sched.h>

#include "../lib/error_functions.h"

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

static int childFunc(void *arg)
{
    if (close(*((int *) arg)) == -1)
        errExit("close");

    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [string]\n", argv[0]);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
        errExit("open");

    int flags = 0;
    if (argc != 1)
        flags |= CLONE_FILES;

    const int STACK_SIZE = 65536;
    char *stack = malloc(STACK_SIZE);
    if (stack == NULL)
        errExit("malloc");
    char *stackTop = stack + STACK_SIZE;

    /* Ignore CHILD_SIG unless its SIGCHLD to ensure zombie is created */

    if (CHILD_SIG != SIGCHLD && CHILD_SIG != 0) {
        struct sigaction sa;
        sa.sa_flags = 0;
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);

        if (sigaction(CHILD_SIG, &sa, NULL) == -1)
            errExit("sigaction");
    }

    if (clone(&childFunc, stackTop, flags | CHILD_SIG, (void *) &fd) == -1)
        errExit("clone");

    if (waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1)
        errExit("waitpid");
    printf("child has terminated\n");

    ssize_t s = write(fd, "x", 1);
    if (s == -1 && errno == EBADF)
        printf("file descriptor %d has been closed\n", fd);
    else if (s == -1)
        printf("write() on file descriptor %d failed "
                "unexpectedly (%s)\n", fd, strerror(errno));
    else
        printf("write() on file descriptor %d succeeded\n", fd);

    exit(EXIT_SUCCESS);
}
