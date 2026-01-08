#include "tlpi_hdr.h" // IWYU pragma: export
#include "unix_sockets.h"
#include "unslib.h"

int main(void)
{
    int sfd = unixConnect(SERV_PATH, SOCK_DGRAM);
    if (sfd == -1)
        errExit("unixConnect");

    const char *msg = "Hello!";
    if (write(sfd, msg, strlen(msg)) != (int) strlen(msg))
        fatal("Partial/Failed write");

    close(sfd);
}
