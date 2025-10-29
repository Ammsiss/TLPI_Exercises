/*
   Initially I thought that I should verify the fd flags before processing
   but this is actually unecessary as you can simply rely on the open and
   write syscalls to handle setting errno and returning the correct value.

   Initial Idea:

   int flags = fcntl(fd, F_GETFL); if (flags == -1) return -1;
   int accessMode = flags & O_ACCMODE;

   if (accessMode != O_WRONLY && accessMode != O_RDWR) {
       errno = EBADF;
       return -1;
   }
*/

#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <bits/posix1_lim.h>

#include "../lib/error_functions.h"

struct data_buffer {
    ssize_t len;
    void *buf;
};

static ssize_t readv_clone(int fd, struct data_buffer *buffers, ssize_t length)
{
    if (length <= 0) {
        errno = EINVAL;
        return -1;
    }

    ssize_t totalBytes = 0;
    for (int i = 0; i < length; ++i)
        totalBytes += buffers[i].len;

    if (totalBytes > SSIZE_MAX) {
        errno = EINVAL;
        return -1;
    }

    ssize_t totalRead = 0;
    for (ssize_t i = 0; i < length; ++i) {
        ssize_t numRead = read(fd, buffers[i].buf, buffers[i].len);
        if (numRead == -1)
            return -1;

        totalRead += numRead;

        if (numRead != buffers[i].len)
            return totalRead;
    }

    return totalRead;
}

// On partial read, return early, returning total number written from  current buffer at that point
static ssize_t writev_clone(int fd, const struct data_buffer *buffers, ssize_t length)
{
    if (length <= 0) {
        errno = EINVAL;
        return -1;
    }

    ssize_t totalBytes = 0;
    for (int i = 0; i < length; ++i)
        totalBytes += buffers[i].len;

    if (totalBytes > SSIZE_MAX) {
        errno = EINVAL;
        return -1;
    }

    ssize_t totalWritten = 0;
    for (ssize_t i = 0; i < length; ++i) {
        ssize_t numWritten = write(fd, buffers[i].buf, buffers[i].len);
        if (numWritten == -1)
            return -1;

        totalWritten += numWritten;

        if (numWritten != buffers[i].len)
            return numWritten;
    }

    return totalWritten;
}

int main(void)
{
    int fd = open("test.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (fd == -1)
        errExit("open");

    struct data_buffer data[2];
    data[0].buf = "Hello, ";
    data[0].len = 7;
    data[1].buf = "World!\n";
    data[1].len = 7;

    int rv = writev_clone(fd, data, 2);
    if (rv == -1)
        errExit("writev_clone");

    if (close(fd) == -1)
        errExit("close");

    fd = open("test.txt", O_RDONLY);

    char buf1[7];
    char buf2[7];

    struct data_buffer buffers[2];
    buffers[0].buf = buf1;
    buffers[0].len = 7;
    buffers[1].buf = buf2;
    buffers[1].len = 7;

    readv_clone(fd, buffers, 2);

    for (int i = 0; i < 2; ++i)
        if (write(STDOUT_FILENO, buffers[i].buf, 7) == -1)
            errExit("write");

    exit(EXIT_SUCCESS);
}
