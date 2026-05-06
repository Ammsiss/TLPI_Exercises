#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include "telnet.h"
#include "inet_sockets.h"
#include "pty_fork.h"
#include "binary_tree.h"

#define BUF_SIZE 256

void sigchld_handler(int sig) {
    (void) sig;

    int saved_errno = errno;

    while (1)
        if (waitpid(-1, NULL, WNOHANG) > 0)
            break;

    errno = saved_errno;
}

void receive_winsize(int clfd_p, struct winsize *ws) {
    /* Retrieve winsize from client. In the future It may be
     * preferable to also retrieve the terminal attributes but
     * for simplicity I'm going to assume they are similar. */
    memset(ws, 0, sizeof(*ws));

    char ws_buf[5];   /* win dimensions shouldn't be more then 4 digits */

    int num_read;
    for (int i = 0; i < 2; ++i ) {
        if ((num_read = read(clfd_p, ws_buf, 5)) == -1)
            errExit("read");

        switch (i) {
        case 0: ws->ws_row = getInt(ws_buf, GN_NONNEG, "buf"); break;
        case 1: ws->ws_col = getInt(ws_buf, GN_NONNEG, "buf"); break;
        default: break;
        }

        /* Send confirmation so we can receive next value */
        if (write(clfd_p, "x", 1) != 1)
            errExit("Failed/Partial write");
    }
}

void handle_client(int clfd, int clfd_p) {
    struct winsize ws;
    receive_winsize(clfd_p, &ws);

    int master_fd;

    int child_pid;
    switch (child_pid = ptyFork(&master_fd, NULL, 0, NULL, &ws)) {
    case -1:
        errExit("ptyFork");
    case 0:
        /* Exec a shell instead of login because it seems that
         * the login program is more tightly integrated with
         * modern security features like PAM and does not work
         * interactively on my macine. */
        execlp("sh", "sh", (char *) NULL);
        errExit("execlp");
    default:
        break;
    }

    int epfd = epoll_create1(0);
    if (epfd == -1)
        errExit("epoll_create");

    struct epoll_event ev;
    /* Monitor pty master fd and the client fd for input */
    ev.data.fd = master_fd;
    ev.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, master_fd, &ev) == -1)
        errExit("epoll_ctl");

    /* Report input and peer socket hangups */
    ev.data.fd = clfd;
    ev.events = EPOLLIN | EPOLLRDHUP;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, clfd, &ev) == -1)
        errExit("epoll_ctl");

    /* Watch priority socket for window size changes */
    ev.data.fd = clfd_p;
    ev.events = EPOLLIN | EPOLLRDHUP;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, clfd_p, &ev) == -1)
        errExit("epoll_ctl");

    char buf[BUF_SIZE];

    struct epoll_event events[2];

    int num_read;
    while (1) {
        int ready = epoll_wait(epfd, events, 2, -1);
        if (ready == -1)
            errExit("epoll_wait");

        for (int i = 0; i < ready; ++i) {
            if (events[i].data.fd == clfd) {
                if (events[i].events & EPOLLRDHUP)
                    return;   /* Client is dead */

                if ((num_read = read(clfd, buf, BUF_SIZE)) == -1)
                    errExit("read");
 
                if (write(master_fd, buf, num_read) != num_read)
                    errExit("write");
            } else if (events[i].data.fd == master_fd) {
                if ((num_read = read(master_fd, buf, BUF_SIZE)) <= 0)
                    return;   /* Slave is dead */

                if (write(clfd, buf, num_read) != num_read)
                    errExit("write");
            } else if (events[i].data.fd == clfd_p) {
                if (events[i].events & EPOLLRDHUP)
                    return;   /* Client is dead */

                receive_winsize(clfd_p, &ws);
                if (ioctl(master_fd, TIOCSWINSZ, &ws) < 0)
                    errExit("ioctl");
            }
        }
    }
}

int main(void) {
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;   /* Restart accept() call if interuptted */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    int sockfd = inetListen(PORT_NUM, 10, NULL);

    char addr_str[NI_MAXSERV + NI_MAXHOST + 5];

    struct sockaddr claddr;
    socklen_t cl_len;

    BTree sessions;
    bt_initialize(&sessions);

    int clfd;
    char session_id[16];
    struct session *session;

    while (1) {
        while (1) {
            clfd = accept(sockfd, &claddr, &cl_len);
            if (clfd == -1) {
                errMsg("accept");
                continue;
            }
            inetAddressStr(&claddr, cl_len, addr_str, sizeof(addr_str));

            if (read(clfd, session_id, 16) != 16)
                continue;

            if (bt_lookup(&sessions, session_id, (void **) &session) == -1) {
                printf("New connection: %s (1/2)\n", addr_str);
                session = malloc(sizeof(struct session));
                session->clfd = clfd;
                bt_add(&sessions, session_id, session);
            } else {
                printf("New connection: %s (2/2)\n", addr_str);
                session->clfd_p = clfd;
                break;
            }
        }

        fflush(stdout);

        switch (fork()) {
        case -1:
            close(session->clfd);   /* Give up on this client */
            close(session->clfd_p);
            break;
        case 0:
            close(sockfd);
            handle_client(session->clfd, session->clfd_p);
            printf("Connection closed: %s\n", addr_str);
            /* No exit handlers and we call fflush() before forking
             * so its fine to call exit() instead of _exit() */
            exit(EXIT_SUCCESS);
        default:
            close(session->clfd);
            close(session->clfd_p);
            if (bt_delete(&sessions, session_id) == -1)
                errExit("bt_delete");
            bt_print(&sessions);
            break;
        }
    }

    close(sockfd);
    exit(EXIT_SUCCESS);
}
