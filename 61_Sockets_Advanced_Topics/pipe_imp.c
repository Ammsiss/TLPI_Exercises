#include "tlpi_hdr.h" // IWYU pragma: export
#include <ctype.h>
#include <sys/socket.h>

#define BUF_SIZE 100

#define R_END 0
#define W_END 1

int pipe_imp(int fd[2])
{
    int sock_fd[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sock_fd) == -1)
        return -1;

    if (shutdown(sock_fd[0], SHUT_WR) == -1) {
        close(sock_fd[0]);
        close(sock_fd[1]);
        return -1;
    }

    if (shutdown(sock_fd[1], SHUT_RD) == -1) {
        close(sock_fd[0]);
        close(sock_fd[1]);
        return -1;
    }

    fd[0] = sock_fd[0];
    fd[1] = sock_fd[1];

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s msg", argv[0]);

    if (strlen(argv[1]) > BUF_SIZE -1)
        cmdLineErr("msg must be less then 100 characters");

    int num_read;

    char buf[BUF_SIZE];
    strcpy(buf, argv[1]);

    int parent_child[2];
    if (pipe_imp(parent_child) == -1)
        errExit("pipe");
    int child_parent[2];
    if (pipe_imp(child_parent) == -1)
        errExit("pipe");

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
        if (close(parent_child[W_END]) == -1)
            errExit("close");
        if (close(child_parent[R_END]) == -1)
            errExit("close");
        if ((num_read = read(parent_child[R_END], buf, BUF_SIZE)) <= 0)
            fatal("Child: Error/EOF on read");
        for (char *c = buf; *c != '\0'; ++c)
            *c = toupper(*c);
        if (write(child_parent[W_END], buf, num_read) != num_read)
            fatal("Child: Failed/Partial write");
        _exit(EXIT_SUCCESS);
    default:
        if (close(parent_child[R_END]) == -1)
            errExit("close");
        if (close(child_parent[W_END]) == -1)
            errExit("close");
        if (write(parent_child[W_END], buf, strlen(buf) + 1) != (int) strlen(buf) + 1)
            fatal("Parent: Failed/Partial write");
        if (read(child_parent[R_END], buf, BUF_SIZE) <= 0)
            fatal("Parent: Error/EOF on read");
        printf("%s\n", buf);
    }
}
