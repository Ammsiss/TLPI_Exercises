#include "tlpi_hdr.h" // IWYU pragma: export

#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UDDS_PATH "/tmp/udds_test_sock"
#define UNADDR_SIZE sizeof(struct sockaddr_un)

void *threadFunc(void *arg)
{
    int sfd;
    if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
        errExit("socket");

    struct sockaddr_un svaddr;

    memset(&svaddr, 0, UNADDR_SIZE);
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, UDDS_PATH, sizeof(svaddr.sun_path) - 1);

    struct timespec ts = { /*tv_sec=*/0, /*tv_nsec=*/2500000 };

    char c = 'x';
    while (1) {
        if (sendto(sfd, &c, 1, 0, (struct sockaddr *) &svaddr,
                UNADDR_SIZE) != 1)
            fatal("sendto");

        if (clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL) != 0)
            errExit("clock_nanosleep");

        putchar('s');
        fflush(stdout);
    }

    return arg;
}

int main(void)
{
    if (remove(UDDS_PATH) == -1 && errno != ENOENT)
        errExit("remove-%s", UDDS_PATH);

    int sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    struct sockaddr_un saddr;

    memset(&saddr, 0, UNADDR_SIZE);
    saddr.sun_family = AF_UNIX;
    strncpy(saddr.sun_path, UDDS_PATH, sizeof(saddr.sun_path));

    if (bind(sfd, (struct sockaddr *) &saddr, UNADDR_SIZE) == -1)
        errExit("bind - server");

    pthread_t thread;

    int s;
    if ((s = pthread_create(&thread, NULL, threadFunc, NULL)) != 0)
        errExitEN(s, "pthread_create");

    /* Loop reading 1 request every second */

    struct timespec ts = { /*tv_sec=*/0, /*tv_nsec=*/10000000 };

    char c;
    while (1) {
        int num_read = recv(sfd, &c, 1, 0);
        if (num_read == -1)
            errExit("recvfrom");

        putchar('r');
        fflush(stdout);

        if (clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL) != 0)
             errExit("clock_nanosleep");
    }

    if ((s = pthread_join(thread, NULL)) != 0)
        errExitEN(s, "pthread_join");
    return EXIT_SUCCESS;
}
