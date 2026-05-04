#include "tlpi_hdr.h" // IWYU pragma: export

#include <time.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main(void)
{
    int nfds = 5;

    int epfd = epoll_create(nfds);
    if (epfd == -1)
        errExit("epoll_create");

    /* Add 10 pipes to the interest list and write 1 byte
     * each into them */

    struct epoll_event event;
    int pfd[2];
    char c = 'x';

    for (int i = 0; i < nfds; ++i) {
        if (pipe(pfd) == -1)
            errExit("pipe");

        if (write(pfd[1], &c, 1) == -1)
            errExit("write");

        event.data.fd = pfd[0];
        event.events = EPOLLIN;

        if (epoll_ctl(epfd, EPOLL_CTL_ADD, pfd[0], &event) == -1)
            errExit("epoll_ctl");
    }

    /* Call epoll_wait() with a small maxevents value */

    while (1) {
        int ready = epoll_wait(epfd, &event, 1, -1);
        if (ready == -1)
            errExit("epoll_wait");

        if (ready != 0)
            printf("fd = %d\n", event.data.fd);

        struct timespec ts = { 1, 0 };
        if (nanosleep(&ts, NULL) == -1)
            errExit("nanosleep");
    }
}
