#define _GNU_SOURCE

#include <sys/socket.h>
#include <netinet/in.h>

#include "tlpi_hdr.h"
#include "echo.h"

char buf[BUF_SIZE];

int main(void)
{
    struct sockaddr_in sockaddr;

    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        errExit("socket");

    if (connect(sockfd, (struct sockaddr *) &sockaddr,
                sizeof(struct sockaddr_in)) == -1)
        errExit("connect");

    strcpy(buf, "Hello from the tcp client!");

    if (write(sockfd, buf, strlen(buf) + 1) != (ssize_t) strlen(buf) + 1)
        errExit("write");

    printf("Sent message: %s\n", buf);

    if (read(sockfd, buf, strlen(buf) + 1) != (ssize_t) strlen(buf) + 1)
        errExit("read");

    printf("Server response: %s\n", buf);

    close(sockfd);
}
