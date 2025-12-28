#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/socket.h>
#include <sys/un.h>

int create_udgram(struct sockaddr_un *addr, char *path, int abstract)
{
    int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock == -1)
        errExit("socket");

    /* Not really necessary but good practice. If you had a
     * remove call right after then it would be necessary */

    if (strlen(path) > sizeof(addr->sun_path) - 2) {
        errno = EINTR;
        return -1;
    }

    memset(addr, 0, sizeof(struct sockaddr_un));
    addr->sun_family = AF_UNIX;

    if (abstract)
        strncpy(&addr->sun_path[1], path, sizeof(addr->sun_path) - 2);
    else
        strncpy(addr->sun_path, path, sizeof(addr->sun_path) - 1);

    if (bind(sock, (struct sockaddr*) addr, sizeof(struct sockaddr_un)) == -1)
        return -1;

    return sock;
}

int main(void)
{
    struct sockaddr_un addr_a;
    int sock_a = create_udgram(&addr_a, "abc", 1);
    if (sock_a == -1)
        errExit("create_udgram");

    struct sockaddr_un addr_b;
    int sock_b = create_udgram(&addr_b, "def", 1);
    if (sock_b == -1)
        errExit("create_udgram");

    /* Connect a -> b. This means that a can only read messages
     * that arrive from socket b. */

    if (connect(sock_a, (struct sockaddr *) &addr_b,
                sizeof(struct sockaddr_un)) == -1)
        errExit("connect");

    int sock_c = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_c == -1)
        errExit("socket");

    /* Attempt a send from c -> b */

    char c1 = 'x';
    if (sendto(sock_c, &c1, 1, 0, (struct sockaddr *) &addr_a,
            sizeof (struct sockaddr_un)) == -1)
        errExit("sendto");

    char c2;
    if (read(sock_a, &c2, 1) != sizeof(char))
        errExit("read");

    if (close(sock_a) == -1)
        errExit("close");
    if (close(sock_b) == -1)
        errExit("close");
    if (close(sock_c) == -1)
        errExit("close");

    return EXIT_SUCCESS;
}
