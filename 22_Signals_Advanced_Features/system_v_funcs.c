#define _GNU_SOURCE

#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t imp_sigset(int sig, sighandler_t disp)
{
    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, sig);

    int set = (disp == SIG_HOLD) ? SIG_BLOCK : SIG_UNBLOCK;
    if (sigprocmask(set, &blockMask, NULL) == -1)
        return SIG_ERR;

    if (set == SIG_BLOCK)
        return SIG_HOLD;

    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = disp;

    struct sigaction osa;

    if (sigaction(sig, &sa, &osa) == -1)
        return SIG_ERR;

    return osa.sa_handler;
}

int imp_sighold(int sig)
{
    sigset_t blockMask;
    sigemptyset(&blockMask);
    sigaddset(&blockMask, sig);

    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
        return -1;

    return 0;
}

int imp_sigrelse(int sig)
{
    sigset_t removeMask;
    sigemptyset(&removeMask);
    sigaddset(&removeMask, sig);

    if (sigprocmask(SIG_UNBLOCK, &removeMask, NULL) == -1)
        return -1;

    return 0;
}

int imp_sigignore(int sig)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;

    if (sigaction(sig, &sa, NULL) == -1)
        return -1;

    return 0;
}

int imp_sigpause(int sig)
{
    sigset_t newSet;
    if (sigprocmask(SIG_BLOCK, NULL, &newSet) == -1)
        return -1;

    sigdelset(&newSet, sig);

    return sigsuspend(&newSet);
}
