#include "tlpi_hdr.h" // IWYU pragma: export

#include <signal.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/random.h>

#include "telnet.h"
#include "inet_sockets.h"
#include "tty_functions.h"

#define BUF_SIZE 256

static struct termios initial_term;

void reset_term(void) {
    if (tcsetattr(STDIN_FILENO, TCSANOW, &initial_term) == -1)
        errExit("tcsetattr");
}

void send_winsize(int svfd_p) {
    /* Get winsize for slave */
    struct winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
        errExit("ioctl-TIOCGWINSZ");

    char conf;
    char ws_buf[5] = "";

    for (int i = 0; i < 2; ++i) {
        switch (i) {
        case 0: snprintf(ws_buf, 5, "%d", ws.ws_row); break;
        case 1: snprintf(ws_buf, 5, "%d", ws.ws_col); break;
        default: break;
        }

        if (write(svfd_p, ws_buf, 5) != 5)
            errExit("Failed/Partial write");

        /* Wait for server read confirmation */
        if (read(svfd_p, &conf, 1) != 1)
            errExit("Failed/Partial read");
    }
}

static volatile sig_atomic_t sigwinch_caught = 0;

void sigwinch_handler(int sig) {
    (void) sig;

    sigwinch_caught = 1;
}

int main(int argc, char **argv)
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s hostname", argv[0]);

    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = sigwinch_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGWINCH, &sa, NULL) == -1)
        errExit("sigaction");

    /* Set raw mode and an exit handler to reset */
    ttySetRaw(STDIN_FILENO, &initial_term);
    if (atexit(reset_term) == -1)
        errExit("atexit");

    char session_id[16];
    if (getrandom(session_id, sizeof(session_id), 0) != 16)
        errExit("getrandom: Failed/Partial result");

    /* Connect to server */
    int svfd = inetConnect(argv[1], PORT_NUM, SOCK_STREAM);
    if (svfd == -1)
        errExit("inetConnect");
    if (write(svfd, session_id, 16) != 16)
        errExit("Failed/Partial write");

    int svfd_p = inetConnect(argv[1], PORT_NUM, SOCK_STREAM);
    if (svfd_p == -1)
        errExit("inetConnect");
    if (write(svfd_p, session_id, 16) != 16)
        errExit("Failed/Partial write");

    send_winsize(svfd_p);

    int epfd = epoll_create1(0);
    if (epfd == -1)
        errExit("epoll_create");

    struct epoll_event ev;
    /* Monitor pty master fd and the client fd for input */
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    /* Report input and peer socket hangups */
    ev.events = EPOLLIN | EPOLLRDHUP;
    ev.data.fd = svfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, svfd, &ev);

    char buf[BUF_SIZE];

    struct epoll_event events[2];

    int num_read;
    while (1) {
        if (sigwinch_caught) {
            send_winsize(svfd_p);
            sigwinch_caught = 0;
        }

        int ready = epoll_wait(epfd, events, 2, -1);
        if (ready == -1 && errno == EINTR)
            continue;
        else if (ready == -1)
            errExit("epoll_wait");

        for (int i = 0; i < ready; ++i) {
            if (events[i].data.fd == svfd) {
                if (events[i].events & EPOLLRDHUP)
                    exit(EXIT_SUCCESS);   /* Server child is dead */

                if ((num_read = read(svfd, buf, BUF_SIZE)) == -1)
                    errExit("read");

                if (write(STDOUT_FILENO, buf, num_read) != num_read)
                    errExit("write");
            } else if (events[i].data.fd == STDIN_FILENO) {
                if ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) <= 0)
                    errExit("read");

                if (write(svfd, buf, num_read) != num_read)
                    errExit("write");
            }
        }
    }
}
