////////////////////////////////////////////////////////////////////////
// 5.14 - 5-4. dup() and dup2() clones.
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define MIN_FD 0

static int dup_clone(int fd)
{
    return fcntl(fd, F_DUPFD, MIN_FD);
}

static int dup2_clone(int oldFd, int newFd)
{
    if (oldFd == newFd)
    {
        if (fcntl(newFd, F_GETFD) == -1)
        {
            errno = EBADF;
            return -1;
        }
    }

    if (fcntl(newFd, F_GETFD) != -1)
        close(newFd);

    return fcntl(oldFd, F_DUPFD, newFd);
}

int main(void)
{
    int fd1 = dup_clone(STDOUT_FILENO);
    write(fd1, "hello\n", 6);

    int fd2 = dup(STDOUT_FILENO);
    write(fd2, "hello\n", 6);

    dup2_clone(STDOUT_FILENO, 20);
    write(20, "hello\n", 6);

    return EXIT_SUCCESS;
}
