#include "tlpi_hdr.h" // IWYU pragma: export

#include "inet_sockets.h"

int main(void)
{
    int sfd;
    char c = 'x';

    /* Specifying 'host' as NULL uses loop back address.

       (inetListen() achieves this by calling getaddrinfo with
       the AI_PASSIVE flag NOT set and 'node' set to NULL.) */

    if ((sfd = inetConnect("localhost", "50000", SOCK_STREAM)) == -1)
        errExit("inetConnect");

    if (write(sfd, &c, 1) != 1)
        errExit("write");

    if (close(sfd) == -1)
        errExit("close");
}
