#include "tlpi_hdr.h" // IWYU pragma: export

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(void)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;   /* IPV4 or IPV6 */
    hints.ai_flags = 0;   /* Passive socket structure */
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;

    /* If 'host' is null and AI_PASSIVE is set; use INADDR_ANY */
    struct addrinfo *result;

    int rv;
    if ((rv = getaddrinfo("www.youtube.com", "https", &hints, &result)) != 0)
        fatal("%s", gai_strerror(rv));

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
        if ((rv = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST,
                serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV)) != 0)
            fatal("%s", gai_strerror(rv));
        printf("%s - %s\n", host, serv);
    }
}
