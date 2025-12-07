#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#include <fcntl.h>

#define FILE_PATH "/tmp/alternate_locks.txt"
#define FILE_LEN  80001

int main()
{
    int fd;
    if ((fd = open(FILE_PATH, O_CREAT | O_RDWR, 0600)) == -1)
        errExit("open");
    if (ftruncate(fd, FILE_LEN) == -1)
        errExit("ftruncate");

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_pid = 0;
    fl.l_len = 1;
    fl.l_whence = SEEK_SET;

    int cmd = F_SETLK;

    for (int i = 0; i < FILE_LEN; i += 2) {
        fl.l_start = i;

        if (fcntl(fd, cmd, &fl) == -1)
            errExit("fcntl");
    }

    printf("(%d) locks ready on file %s", getpid(), FILE_PATH);
    pause();   /* Don't exit so locks stay in place */
}
