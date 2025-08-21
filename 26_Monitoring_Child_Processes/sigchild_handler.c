#define _GNU_SOURCE

#include <stdlib.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"
#include "print_wait_status.h"

static volatile int childrenLeft = 0;

static void handler(int sig)
{
    int savedErrno = errno;

    printf("Handler: Caught %d (%s)\n", sig, strsignal(sig));

    int pid;
    int status;
    while (1) {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid <= 0)
            break;

        printWaitStatus(NULL, status);
        printf("PID=%d reaped\n", pid);

        --childrenLeft;
    }
    if (pid == -1 && errno != ECHILD)
        errExit("waitpid");

    sleep(5);   /* Wait for more SIGCHLD signals */
    printf("Handler returning...\n");

    errno = savedErrno;
}

int main(int argc, char **argv)
{
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0))
        usageErr("%s secs ...", argv[0]);

    childrenLeft = argc - 1;

    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = &handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);

    sigset_t oldMask;

    if (sigprocmask(SIG_BLOCK, &blockMask, &oldMask) == -1)
        errExit("sigprocmask");

    for (int j = 1; j < argc; ++j) {

        int seconds = getNum(argv[j], "Arguments must be integers");

        switch (fork()) {
        case -1:
            errExit("fork");
        case 0:
            sleep(seconds);
            printf("(PID=%d) Child %d exiting...\n", getpid(), j);
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    int sigCount = 0;
    while (childrenLeft > 0) {
        if (sigsuspend(&oldMask) == -1 && errno == ECHILD)
            errExit("sigsuspend");
        ++sigCount;
    }

    printf("No more children left. SIGCHILD was caught %d times\n", sigCount);

    exit(EXIT_SUCCESS);
}
