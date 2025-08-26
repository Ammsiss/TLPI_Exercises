#define _GNU_SOURCE

#include <string.h>
#include <unistd.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [file]\n", argv[0]);

    if (argc == 1) {
        if (acct(NULL) == -1)
            errExit("acct");
    } else
        if (acct(argv[1]) == -1)
            errExit("acct");

    exit(EXIT_SUCCESS);
}
