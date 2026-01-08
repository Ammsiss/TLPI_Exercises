#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <errno.h>

#include "unix_sockets.h"

int unixConnect(const char *path, int type)
{
    if (strlen(path) >= 108) {
        errno = EINVAL;
        return -1;
    }

    int sfd = socket(AF_UNIX, type, 0);
    if (sfd == -1)
        return -1;

    struct sockaddr_un saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_un));
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, path);

    socklen_t addrlen = sizeof(struct sockaddr_un);

    if (connect(sfd, (struct sockaddr *) &saddr, addrlen) == -1)
        return -1;

    return sfd;
}

int unixListen(const char *path, int backlog)
{
    if (strlen(path) >= 108) {
        errno = EINVAL;
        return -1;
    }

    struct sockaddr_un saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_un));
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, path);

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        return -1;

    if (bind(sfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_un)) == -1)
        return -1;

    if (listen(sfd, backlog) == -1)
        return -1;

    return sfd;
}

int unixBind(const char *path, int type)
{
    if (strlen(path) >= 108) {
        errno = EINVAL;
        return -1;
    }

    int sfd = socket(AF_UNIX, type, 0);
    if (sfd == -1)
        return -1;

    struct sockaddr_un saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_un));
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, path);

    if (bind(sfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_un)) == -1)
        return -1;

    return sfd;
}
