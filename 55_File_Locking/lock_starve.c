#define _GNU_SOURCE     /* To get definitions of 'OFD' locking commands */

#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#define FILE_NAME "/tmp/test_file.txt"

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s iterations\n", argv[0]);

    int iterations = getInt(argv[1], GN_GT_0, "iterations");

    int fd;
    if ((fd = open(FILE_NAME, O_RDWR | O_CREAT, 0600)) == -1)
        errExit("open");
    if (ftruncate(fd, 1) == -1)
        errExit("ftruncate");

    int cmd = F_SETLKW;
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_pid = 0;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_whence = SEEK_SET;

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        for (int i = 0; i < iterations; ++i) {
            switch (fork()) {
            case -1:
                errExit("fork");
            case 0:
            {
                cmd = F_SETLK;
                fl.l_type = F_RDLCK;
                if (fcntl(fd, cmd, &fl) == -1)
                    errExit("fcntl");
                printf("(%d) Acquired read lock\n", getpid());
                sleep(6);

                fl.l_type = F_UNLCK;
                if (fcntl(fd, cmd, &fl) == -1)
                    errExit("fcntl");
                printf("(%d) Released read lock\n", getpid());
                _exit(EXIT_SUCCESS);
            }
            default:
                sleep(5);
                break;
            }
        }
        while (1) {
            if (waitpid(-1, NULL, 0) == -1) {
                if (errno == ECHILD)
                    break;
                else
                    errExit("waitpid");
            }
        }
        _exit(EXIT_SUCCESS);
    default:
        break;
    }

    sleep(1);   /* Wait so that first read lock goes through */

    printf("(%d) Requesting write lock\n", getpid());

    if (fcntl(fd, cmd, &fl) == -1)
        errExit("fcntl");
    if (errno == EDEADLK)
        errExit("fcntl");

    printf("(%d) Acquired write lock\n", getpid());

    if (waitpid(-1, NULL, 0) == -1)
        errExit("waitpid");
    exit(EXIT_SUCCESS);
}
