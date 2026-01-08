#include "tlpi_hdr.h" // IWYU pragma: export
#include "unslib.h"
#include "unix_sockets.h"

int main(void)
{
    int sfd = unixBind(SERV_PATH, SOCK_DGRAM);
    if (sfd == -1)
        errExit("unixBind");

    char msg[100];
    while (1) {
        int num_read = read(sfd, msg, sizeof(msg));
        if (num_read == -1)
            errExit("read");

        msg[num_read - 1] = '\0';
        printf("Received %s\n", msg);
    }
}
