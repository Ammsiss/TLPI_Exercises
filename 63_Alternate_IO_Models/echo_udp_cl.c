#define _GNU_SOURCE

#include <sys/socket.h>
#include <netinet/in.h>

#include "tlpi_hdr.h"
#include "echo.h"

char buf[BUF_SIZE];

int main(void)
{
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1)
        errExit("socket");

    struct sockaddr_in claddr;

    memset(&claddr, 0, sizeof(struct sockaddr_in));
    claddr.sin_addr.s_addr = INADDR_ANY;
    claddr.sin_port = htons(PORT);
    claddr.sin_family = AF_INET;

    strcpy(buf, "Hello from the udp client!");

    if (sendto(sock_fd, &buf, strlen(buf) + 1, 0, (struct sockaddr *) &claddr,
            sizeof(struct sockaddr_in)) != (ssize_t) strlen(buf) + 1)
        errExit("Failed/Partial write with sendto()\n");

    printf("Sent message: %s\n", buf);

    if (recvfrom(sock_fd, &buf, strlen(buf) + 1, 0, NULL, NULL) !=
            (ssize_t) strlen(buf) + 1)
        errExit("Failed/Partial read with recvfrom()\n");

    printf("Server response: %s\n", buf);

    close(sock_fd);
}
