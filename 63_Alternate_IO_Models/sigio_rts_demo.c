#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include "error_functions.h"
#include "tty_functions.h"

static volatile sig_atomic_t sigint_caught = 0;

void sigint_handler(int sig) {
    (void) sig;

    sigint_caught = 1;
}

int main(void) {
    if (ttySetCbreak(STDIN_FILENO, NULL) == -1)
        errExit("ttySetCbreak");

    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGRTMIN + 1);

    /* BLock our signal so that we can process it
     * safely with sigtimedwait */

    if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)
        errExit("sigprocmask");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1)
        errExit("fcntl");

    if (fcntl(STDIN_FILENO, F_SETSIG, SIGRTMIN + 1) == -1)
        errExit("fcntl");

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1)
        errExit("fcntl");

    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK | O_ASYNC) == -1)
        errExit("fcntl");

    int c;
    siginfo_t si;
    size_t cnt = 0;
    struct timespec sigtime_ts = { /*tv_sec=*/0, /*tv_nsec=*/0 };
    struct timespec sleep_ts = { /*tv_sec=*/0, /*tv_nsec=*/100000000 };

    while (1) {
        if (sigint_caught)
            break;

        if (sigtimedwait(&block_set, &si, &sigtime_ts) == -1) {
            if (errno != EAGAIN)
                errExit("sigtimedwait");
        } else {
            printf("IO signal caught for fd %d\n", si.si_fd);

            printf("si_code = ");
            switch(si.si_code) {
            case POLL_IN:  printf("POLL_IN");  break;
            case POLL_OUT: printf("POLL_OUT"); break;
            case POLL_MSG: printf("POLL_MSG"); break;
            case POLL_ERR: printf("POLL_ERR"); break;
            case POLL_PRI: printf("POLL_PRI"); break;
            case POLL_HUP: printf("POLL_HUP"); break;
            }
            printf("\n");

            while (read(STDIN_FILENO, &c, 1) > 0)
                printf("cnt: %ld; read %c\n", cnt, c);
        }

        if (nanosleep(&sleep_ts, NULL) == -1) {
            if (errno == EINTR)
                continue;
            errExit("nanosleep");
        }
        ++cnt; /* +1 unit of work completed */
    }

    return EXIT_SUCCESS;
}
