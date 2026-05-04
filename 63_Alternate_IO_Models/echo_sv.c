#define _GNU_SOURCE

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "tlpi_hdr.h"
#include "echo.h"

#define MAX_EVENTS 2

char buf[BUF_SIZE];

int main(void)
{
    struct sockaddr_in svaddr;

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY;
    svaddr.sin_port = htons(PORT);

    /* Create and bind a tcp and udp socket */

    int dgram_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (dgram_sock == -1)
        errExit("socket");

    if (bind(dgram_sock, (const struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_in)) == -1)
        errExit("bind");

    int stream_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (stream_sock == -1)
        errExit("socket");

    if (bind(stream_sock, (const struct sockaddr *) &svaddr,
                sizeof(struct sockaddr_in)) == -1)
        errExit("bind");

    if (listen(stream_sock, 5) == -1)
        errExit("listen");

    /* Add tcp and udp sockets to epoll interest list */

    int epfd = epoll_create(2);
    if (epfd == -1)
        errExit("epoll_create");

    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = dgram_sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, dgram_sock, &event) == -1)
        errExit("epoll_ctl");

    event.data.fd = stream_sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, stream_sock, &event) == -1)
        errExit("epoll_ctl");

    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int num_ready = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (num_ready == -1)
            errExit("epoll_wait");

        for (int i = 0; i < MAX_EVENTS; ++i) {
            if (events[i].events & EPOLLIN) {
                if (events[i].data.fd == dgram_sock) {
                    struct sockaddr_in claddr;
                    socklen_t len = sizeof(struct sockaddr);

                    int num_read = recvfrom(dgram_sock, buf, 100, 0,
                            (struct sockaddr *) &claddr, &len);
                    if (num_read == -1)
                        errExit("recvfrom");

                    // test
                    printf("Received: %s\n", buf);

                    if (sendto(dgram_sock, buf, num_read, 0,
                                (struct sockaddr *) &claddr, len) != num_read)
                        fprintf(stderr, "Failed/Partial write with sendto()\n");
                } else if (events[i].data.fd == stream_sock) {
                    int cl_sock = accept(stream_sock, NULL, NULL);
                    if (cl_sock == -1)
                        errExit("accept");

                    int num_read = read(cl_sock, buf, 100);
                    if (num_read == -1)
                        errExit("num_read");

                    // test
                    printf("Received: %s\n", buf);

                    if (write(cl_sock, buf, num_read) != num_read)
                        fprintf(stderr, "Failed/Partial write with write()\n");

                    close(cl_sock);
                }
            }
        }
    }


    close(dgram_sock);
    close(stream_sock);
    return 0;
}
