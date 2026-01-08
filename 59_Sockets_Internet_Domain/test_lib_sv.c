#include "tlpi_hdr.h" // IWYU pragma: export

#include "inet_sockets.h"

int main(void)
{
    int sfd;
    socklen_t addrlen;

    /* Does a passive bind (INADDR_ANY:50000). This
       includes '127.0.0.1:50000' used in the client. */

    if ((sfd = inetListen("50000", 10, &addrlen)) == -1)
        errExit("inetListen");

    struct sockaddr_in claddr;
    socklen_t sock_len = sizeof(struct sockaddr_in);

    /* +4 to fit the formatting of the return string */
    int addr_str_len = NI_MAXHOST + NI_MAXSERV + 4;
    char addr_str[addr_str_len];

    while (1) {
        int con_sfd = accept(sfd, (struct sockaddr *) &claddr,
                             &sock_len);

        inetAddressStr((struct sockaddr *) &claddr, sock_len,
                       addr_str, addr_str_len);

        printf("Client connected with info: %s\n", addr_str);

        char c;
        if (read(con_sfd, &c, 1) == -1)
            errExit("read");

        printf("%c\n", c);

        close(con_sfd);
    }
}
