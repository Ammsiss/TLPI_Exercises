#define _GNU_SOURCE

#include "tlpi_hdr.h" // IWYU pragma: export

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

int inetBind(const char *service, struct sockaddr *saddr, socklen_t *addrlen)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;

    struct addrinfo *res;

    if (getaddrinfo(NULL, service, &hints, &res) == -1)
        return -1;

    int sfd;
    int optval = 1;
    struct addrinfo *rp;
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sfd == -1) {
            freeaddrinfo(res);
            return -1;
        }

        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                    sizeof(optval)) == -1) {
            close(sfd);
            freeaddrinfo(res);
            return -1;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        /* Bind failed, close and try next addrinfo structure */

        if (close(sfd) == -1) {
            freeaddrinfo(res);
            return -1;
        }
    }

    if (rp == NULL) {
        freeaddrinfo(res);
        return -1;
    }

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    int err = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, serv,
            NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

    if (err != 0)
        gai_strerror(err);

    printf("Binded %s - %s\n", host, serv);
    if (saddr != NULL)
        memcpy(saddr, rp->ai_addr, rp->ai_addrlen);
    if (addrlen != NULL)
        *addrlen = rp->ai_addrlen;

    freeaddrinfo(res);
    return sfd;
}

int main(void)
{
    struct sockaddr saddr1;
    socklen_t addrlen1;
    int sfd1 = inetBind("50000", &saddr1, &addrlen1);
    if (sfd1 == -1)
        errExit("inetBind");

    struct sockaddr saddr2;
    socklen_t addrlen2;
    int sfd2 = inetBind("50001", &saddr2, &addrlen2);
    if (sfd2 == -1)
        errExit("inetBind");

    /* If you comment this connect call out the message is read successfully.
     * If It stays, then the read simply blocks as the message from the third
     * unconnected datagram socket is simply ignored. */

    if (connect(sfd1, &saddr2, addrlen2) == -1)
        errExit("connect");

    int sfd3 = inetBind("50002", NULL, NULL);
    if (sfd3 == -1)
        errExit("inetBind");

    const char *msg = "Hello!";
    if (sendto(sfd3, msg, strlen(msg), 0, &saddr1,
                addrlen1) != (int) strlen(msg))
        fatal("Partial/failed sendo");

    char msg_buf[100];

    int num_read;
    if ((num_read = read(sfd1, msg_buf, strlen(msg))) == -1)
        errExit("read");

    msg_buf[num_read] = '\0';

    printf("Recieved: \"%s\" - %d byte(s) read\n", msg_buf, num_read);

    close(sfd1);
    close(sfd2);
    close(sfd3);
}
