#include "tlpi_hdr.h" // IWYU pragma: export
#include <ctype.h>
#include <sys/socket.h>

#define BUF_SIZE 100

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s msg", argv[0]);

    if (strlen(argv[1]) > BUF_SIZE -1)
        cmdLineErr("msg must be less then 100 characters");

    int num_read;

    char buf[BUF_SIZE];
    strcpy(buf, argv[1]);

    int fd[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1)
        errExit("socketpair");

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        if (close(fd[0]) == -1)
            errExit("close");
        if ((num_read = read(fd[1], buf, BUF_SIZE)) <= 0)
            fatal("Child: Error/EOF on read");
        for (char *c = buf; *c != '\0'; ++c)
            *c = toupper(*c);
        if (write(fd[1], buf, num_read) != num_read)
            fatal("Child: Failed/Partial write");
        _exit(EXIT_SUCCESS);
    default:
        if (close(fd[1]) == -1)
            errExit("close");
        if (write(fd[0], buf, strlen(buf) + 1) != (int) strlen(buf) + 1)
            fatal("Parent: Failed/Partial write");
        if (read(fd[0], buf, BUF_SIZE) <= 0)
            fatal("Parent: Error/EOF on read");
        printf("%s\n", buf);
    }
}
