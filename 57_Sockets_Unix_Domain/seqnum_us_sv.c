#include "tlpi_hdr.h" // IWYU pragma: export
#include "seqnum_us.h"

#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    struct sockaddr_un svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    strcpy(&svaddr.sun_path[1], SV_SOCK_NAME);
    svaddr.sun_family = AF_UNIX;

    if (bind(sfd, (struct sockaddr*) &svaddr,
                sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    if (listen(sfd, 10) == -1)
        errExit("listen");

    int seqnum = 0;
    int number;

    while (1) {
        int con_sfd = accept(sfd, NULL, NULL);
        if (con_sfd == -1)
            errExit("accept");

        if (read(con_sfd, &number, sizeof(int)) != sizeof(int))
            errExit("Failed/Partial read");
        if (write(con_sfd, &seqnum, sizeof(int)) != sizeof(int))
            errExit("Failed/Partial read");

        seqnum += number;

        if (close(con_sfd) == -1)
            errExit("close");
    }
}
