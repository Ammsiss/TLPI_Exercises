#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

int main(void)
{
    const char *SOCKNAME = "/tmp/mysock";

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0); /* Create socket */
    if (sfd == -1)
        errExit("socket");

    struct sockaddr_un addr;

    /* Using memset() to zero out the entire structure, rather than
    initializing individual fields, ensures that any nonstandard fields that are
    provided by some implementations are also initialized to 0. */

    memset(&addr, 0, sizeof(struct sockaddr_un)); /* Clear structure */
    addr.sun_family = AF_UNIX; /* UNIX domain address */
    strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");
}
