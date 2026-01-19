#include "tlpi_hdr.h" // IWYU pragma: export
#include <netinet/in.h>
#include <sys/socket.h>

#include "inet_sockets.h"

int main(void)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    if (listen(sfd, 10) == -1)
        errExit("listen");

    struct sockaddr_in saddr;
    socklen_t socklen = sizeof(struct sockaddr_in);

    if (getsockname(sfd, (struct sockaddr *) &saddr, &socklen) == -1)
        errExit("getsockname");

    char addr_str[IS_ADDR_STR_LEN];
    inetAddressStr((struct sockaddr *) &saddr, socklen, addr_str,
            IS_ADDR_STR_LEN);

    /* The ip should be wildcard and port in the ephem range
     * which is usually defined as 32768–60999 */

    printf("%s\n", addr_str);
}
