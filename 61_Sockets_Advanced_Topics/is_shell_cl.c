#define _DEFAULT_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

#include "is_shell.h"
#include "dynamic_array.h"

va_list ap;

void errExit(const char *fmt, ...)
{
    va_start(ap, fmt);

    fflush(stdout);
    fprintf(stderr, "\x1b[1;31mERROR\x1b[m: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, " [%s]\n", strerror(errno));

    va_end(ap);
}

int inetConnect(char *host)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICHOST;

    struct addrinfo *res;
    struct addrinfo *rp;

    int rv = getaddrinfo(host, PORT, &hints, &res);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    int sfd;

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

        close(sfd);
    }

    freeaddrinfo(res);

    return (rp == NULL) ? -1 : sfd;
}

int main(int argc, char **argv)
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s server-host prog [arg ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Convert cli args to string command for server */

    char cmd_buf[MAX_CMD + 1] = "";
    int cmd_len = 1; /* start at 1 for final '\0' */

    for (int i = 2; i < argc; ++i)
        cmd_len += strlen(argv[i]) + 1; /* +1 for ' ' between args */

    if (cmd_len > MAX_CMD) {
        fprintf(stderr, "Cmdline error: max cmd len = %d\n", MAX_CMD);
        return EXIT_FAILURE;
    }

    for (int i = 2; i < argc; ++i) {
        strcat(cmd_buf, argv[i]);
        strcat(cmd_buf, " ");
    }

    /* Connect and write command to server */

    int sfd = inetConnect(argv[1]);
    if (sfd == -1) {
        fprintf(stderr, "inetConnect: Could not find server\n");
        return EXIT_FAILURE;
    }

    if (write(sfd, cmd_buf, cmd_len) != cmd_len) {
        fprintf(stderr, "Client: Failed/Partial write");
        return EXIT_FAILURE;
    }

    if (shutdown(sfd, SHUT_WR) == -1)
        errExit("shutdown");

    DynArray da;
    if (da_init(&da, 0, sizeof(char)) == -1) {
        fprintf(stderr, "da_init");
        return EXIT_FAILURE;
    }

    char c;
    int num_read;
    while (1) {
        if ((num_read = read(sfd, &c, 1)) == -1)
            errExit("client: read");

        if (num_read == 0)
            break;

        char *element = da_push(&da);
        if (element == NULL) {
            fprintf(stderr, "da_push");
            return EXIT_FAILURE;
        }
        *element = c;
    }

    *((char *) da_push(&da)) = '\0';
    printf("%s", (char *) da.data);

    close(sfd);
    da_free(&da);
}
