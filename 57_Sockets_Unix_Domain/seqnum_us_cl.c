#include "tlpi_hdr.h" // IWYU pragma: export
#include "seqnum_us.h"

#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s number", argv[0]);

    int number = getInt(argv[1], GN_GT_0, "number");

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    struct sockaddr_un svaddr;
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    strcpy(&svaddr.sun_path[1], SV_SOCK_NAME);
    svaddr.sun_family = AF_UNIX;

    if (connect(sfd, (struct sockaddr*) &svaddr,
                sizeof(struct sockaddr_un)) == -1)
        errExit("connect");

    if (write(sfd, &number, sizeof(int)) != sizeof(int))
        errExit("Failed/Partial write");
    if (read(sfd, &number, sizeof(int)) != sizeof(int))
        errExit("Failed/Partial read");

    printf("Previous value: %d\n", number);

    if (close(sfd) == -1)
        errExit("close");
    return EXIT_SUCCESS;
}
