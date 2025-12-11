/* Header file for Listing 20-4 */

/* signal_functions.h

   Header file for signal_functions.c.

   Modified by Junji Tai
*/

#ifndef SIGNAL_FUNCTIONS_H
#define SIGNAL_FUNCTIONS_H

#include <stdio.h>
#include <sys/types.h>
#include <bits/types/sigset_t.h>

void printSigset(FILE *of, const char *ldr, const sigset_t *mask);
int printSigMask(FILE *of, const char *msg);
int printPendingSigs(FILE *of, const char *msg);

#endif
