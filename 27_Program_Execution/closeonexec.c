#include <unistd.h>
#include <fcntl.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    /* If any string is specified, turn on close-on-exec flag */

    if (argc != 1) {
        int flags = fcntl(STDOUT_FILENO, F_GETFD);
        if (flags == -1)
            errExit("fcntl - F_GETFD");

        flags |= FD_CLOEXEC;
        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
            errExit("fcntl - F_SETFD");
    }

    execlp("lsd", "lsd", (char *) NULL);
    errExit("execl");
}
