/* Listing 20-4 */

/* signal_functions.c

   Various useful functions for working with signals.

   Modified by Junji Tai 2025
*/

#define _GNU_SOURCE

#include "tlpi_hdr.h" // IWYU pragma: export
#include "signal_functions.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>

/* NOTE: All of the following functions employ fprintf(), which
   is not async-signal-safe (see Section 21.1.2). As such, these
   functions are also not async-signal-safe (i.e., beware of
   indiscriminately calling them from signal handlers). */

    /* Print list of signals within a signal set */
void printSigset(FILE *of, const char *ldr, const sigset_t *mask)
{
    int cnt = 0;
    for (int sig = 1; sig < NSIG; sig++) {
        if (sigismember(mask, sig)) {
            cnt++;
            fprintf(of, "%s%d (%s)\n", ldr, sig, strsignal(sig));
        }
    }

    if (cnt == 0)
        fprintf(of, "%s<empty signal set>\n", ldr);
}
   /* Print mask of blocked signals for this process */
int printSigMask(FILE *of, const char *msg)
{
    sigset_t currMask;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
        return -1;

    printSigset(of, "\t\t", &currMask);

    return 0;
}
   /* Print signals currently pending for this process */
int printPendingSigs(FILE *of, const char *msg)
{
    sigset_t pendingSigs;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigpending(&pendingSigs) == -1)
        return -1;

    printSigset(of, "\t\t", &pendingSigs);

    return 0;
}
