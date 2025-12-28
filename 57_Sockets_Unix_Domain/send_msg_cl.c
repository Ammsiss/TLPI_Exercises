#include "tlpi_hdr.h" // IWYU pragma: export
#include "send_msg.h"

#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s char", argv[0]);
    if (strlen(argv[1]) != 1)
        usageErr("%s char", argv[0]);

    int c = argv[1][0];

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    struct sockaddr_un svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_un));

    svaddr.sun_family = AF_UNIX;

    /* Make sure to start at index 1 */
    /* (sun_path[0] already set to \0 by memset */

    strcpy(&svaddr.sun_path[1], SERV_SOCK_NAME);

    if (connect(sfd, (struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_un)) == -1)
        errExit("connect");

    if (write(sfd, &c, 1) == -1)
        errExit("write");
    if (read(sfd, &c, 1) == -1)
        errExit("read");

    printf("%c\n", c);

    if (close(sfd) == -1)
        errExit("close");

    return EXIT_SUCCESS;
}
