#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h" // IWYU pragma: export
#include <fcntl.h>

#define FILE_NAME "/tmp/alternate_locks.txt"
#define FILE_LEN 80001
#define ITERATIONS 10000

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s N", argv[0]);

    int N = getInt(argv[1], GN_NONNEG, "N");
    if (N > 40000) {
        fprintf(stderr, "N must be less then or equal to 40'000\n");
        exit(EXIT_FAILURE);
    }

    int fd;
    if ((fd = open(FILE_NAME, O_RDWR)) == -1)
        errExit("open");

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_pid = 0;
    fl.l_len = 1;
    fl.l_start = N * 2;
    fl.l_whence = SEEK_SET;

    int cmd = F_SETLK;

    for (int i = 0; i < ITERATIONS; ++i) {
        if (fcntl(fd, cmd, &fl) == -1) {
            if (errno == EACCES || errno == EAGAIN)   /* Conflicting lock, expected */
                continue;
            else
                errExit("fcntl");
        } else {
            fprintf(stderr, "This should have failed...\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
