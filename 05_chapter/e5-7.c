////////////////////////////////////////////////////////////////////////
// 5.14 - 5-6. writev() & readv() clones
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>

struct data_buffer {
    size_t len;
    void *buf;
};

static ssize_t readv_clone(int fd, struct data_buffer *buffers, ssize_t length)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        return -1;

    int accessMode = flags & O_ACCMODE;
    if (accessMode != O_RDONLY && accessMode != O_RDWR) {
        errno = EBADF;
        return -1;
    }

    ssize_t totalRead = 0;
    for (ssize_t i = 0; i < length; ++i) {
        ssize_t numRead = read(fd, buffers[i].buf, buffers[i].len);
        if (numRead == -1)
            return -1;
        totalRead += numRead;
    }

    return totalRead;
}

int main(void)
{
    int fd = open("test.txt", O_RDONLY);

    char buf1[7];
    char buf2[7];

    struct data_buffer buffers[2];
    buffers[0].buf = buf1;
    buffers[0].len = 7;
    buffers[1].buf = buf2;
    buffers[1].len = 7;

    readv_clone(fd, buffers, 2);

    for (int i = 0; i < 2; ++i)
        write(STDOUT_FILENO, buffers[i].buf, 7);
}

//
// static ssize_t writev_clone(int fd, const struct data_buffer *buffers, size_t length) {
//     int flags = fcntl(fd, F_GETFL);
//     if (flags == -1)
//         return -1;
//
//     int accessMode = flags & O_ACCMODE;
//
//     if (accessMode != O_WRONLY && accessMode != O_RDWR) {
//         errno = EBADF;
//         return -1;
//     }
//
//     ssize_t totalWritten = 0;
//     for (ssize_t i = 0; i < length; ++i) {
//         ssize_t numWritten = write(fd, buffers[i].buf, buffers[i].len);
//
//         if (numWritten == -1)
//             return -1;
//
//         totalWritten += numWritten;
//     }
//
//     return totalWritten;
// }
//
// int main(void) {
//     int fd = open("test.txt", O_CREAT | O_TRUNC | O_RDONLY, 0600);
//     if (fd == -1)
//         errExit("open");
//
//     struct data_buffer data[2];
//     data[0].buf = "Hello, ";
//     data[0].len = 7;
//     data[1].buf = "World!\n";
//     data[1].len = 7;
//
//     int rv = writev_clone(fd, data, 2);
//
//     if (rv == -1)
//         perror("");
// }
