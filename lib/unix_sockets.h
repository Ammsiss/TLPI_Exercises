#ifndef UNIX_SOCKETS_H
#define UNIX_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>

/* Connect to a unix socket specifed in path */

int unixConnect(const char *path, int type);

/* Create a socket and call listen() */

int unixListen(const char *path, int backlog);

/* Create a socket and return it */

int unixBind(const char *path, int type);

#endif
