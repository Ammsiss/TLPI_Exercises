#include "tlpi_hdr.h" // IWYU pragma: export
#include "send_msg.h"

#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    struct sockaddr_un svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_un));

    svaddr.sun_family = AF_UNIX;
    strcpy(&svaddr.sun_path[1], SERV_SOCK_NAME);

    if (remove(SERV_SOCK_NAME) == -1 && errno != ENOENT)
        errExit("remove-%s", SERV_SOCK_NAME);

    if (bind(sfd, (struct sockaddr_un*) &svaddr,
                sizeof(struct sockaddr_un)) == 1)
        errExit("bind");

    if (listen(sfd, 10) == -1)
        errExit("listen");

    while (1) {
        int con_sfd = accept(sfd, NULL, NULL);
        if (con_sfd == -1)
            errExit("accept");

        char c;
        if (read(con_sfd, &c, 1) == -1)
            errExit("read");

        c = toupper(c);

        if (write(con_sfd, &c, 1) == -1)
            errExit("write");

        if (close(con_sfd) == -1)
            errExit("close");
    }

    return EXIT_SUCCESS;
}
