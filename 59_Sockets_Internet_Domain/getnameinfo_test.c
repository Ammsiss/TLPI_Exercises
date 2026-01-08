#include "tlpi_hdr.h" // IWYU pragma: export

#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(void)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 50000;
    addr.sin_family = AF_INET;

    char ip_addr[NI_MAXSERV];

    int rv = (getnameinfo((struct sockaddr*) &addr, sizeof(struct sockaddr_in),
                NULL, 0, ip_addr, NI_MAXSERV, NI_NAMEREQD));
    if  (rv != 0)
        fatal("[gai_strerror] %s", gai_strerror(rv));

    printf("Ip: %s\n", ip_addr);
}
