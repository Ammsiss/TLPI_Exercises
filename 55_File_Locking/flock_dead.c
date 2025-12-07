#include <sys/file.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#define CHILD_FILE "/tmp/child_file.txt"
#define PARENT_FILE "/tmp/parent_file.txt"

#define FIFO "/tmp/sync_fifo"

void unlinkFifo(void)
{
    if (unlink(FIFO) == -1)
        errMsg("unlink");
}

int main(void)
{
    int fd;
    int other_fd;
    int dummy;

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        if ((fd = open(CHILD_FILE, O_CREAT | O_RDWR, 0600)) == -1)
            errExit("open");
        if (flock(fd, LOCK_EX) == -1)
            errExit("flock");
        if ((dummy = open(FIFO, O_WRONLY)) == -1)   /* Sync with parent */
            errExit("open");
        if (close(dummy) == -1)
            errExit("close");

        if ((other_fd = open(PARENT_FILE, O_RDWR)) == -1)
            errExit("open");
        if (flock(other_fd, LOCK_SH) == -1)
            errExit("flock");

        _exit(EXIT_SUCCESS);
    default:
        break;
    }

    if ((fd = open(PARENT_FILE, O_CREAT | O_RDWR, 0600)) == -1)
        errExit("open");
    if (flock(fd, LOCK_EX) == -1)
        errExit("flock");

    if (mkfifo(FIFO, 0600) == -1)
        errExit("mkfifo");
    if (atexit(unlinkFifo) == -1)
        errExit("mkfifo");

    if ((dummy = open(FIFO, O_RDONLY)) == -1)   /* Sync with child */
        errExit("open");
    if (close(dummy) == -1)
        errExit("close");

    if ((other_fd = open(CHILD_FILE, O_RDWR)) == -1)
        errExit("open");
    if (flock(other_fd, LOCK_SH) == -1)
        errExit("flock");

    exit(EXIT_SUCCESS);
}
