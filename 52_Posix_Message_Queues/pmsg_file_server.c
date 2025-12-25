#include "tlpi_hdr.h" /* IWYU pragma: export */
#include "pmsg_file.h"

#include <string.h>
#include <unistd.h>

/* SIGCHLD handler */
static void grimReaper(int sig)
{
    (void) sig;

    /* waitpid() might change 'errno' */
    int savedErrno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;

    errno = savedErrno;
}

static volatile sig_atomic_t quit_req = 0;

/* Just consume SIGKILL */
static void sigInt(int sig)
{
    (void) sig;

    quit_req = 1;
}

/* Executed in child process: serve a single client */
static void serveRequest(const struct request_msg *req)
{
    char msg_data[MSG_SIZE];

    mqd_t client_mqd = mq_open(req->client_pmsg, O_WRONLY);
    if (client_mqd == (mqd_t) -1)
        err_exit("mq_open - server");

    struct response_msg resp;

    int fd = open(req->pathname, O_RDONLY);

    if (fd == -1) {   /* Open failed: send error text */

        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");

        /* Send error reponse back */

        memcpy(msg_data, &resp, sizeof(resp));

        if (mq_send(client_mqd, msg_data, sizeof(resp), 0) == -1)
            err_exit("mq_send");

        _exit(EXIT_FAILURE);   /* and terminate */
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. */

    resp.mtype = RESP_MT_DATA;

    int num_read;
    while ((num_read = read(fd, resp.data, sizeof(resp.data))) > 0) {

        memcpy(msg_data, &resp, sizeof(resp));

        if (mq_send(client_mqd, msg_data, sizeof(resp), 0) == -1) {
            errMsg("mq_send");
            break;
        }
    }

    if (num_read == -1) {
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "read failed");

        /* Send error reponse back */

        memcpy(msg_data, &resp, sizeof(resp));

        if (mq_send(client_mqd, msg_data, sizeof(resp), 0) == -1)
            err_exit("mq_send");

        _exit(EXIT_FAILURE);   /* and terminate */
    }

    /* Send a message of type RESP_MT_END to signify end-of-file */

    resp.mtype = RESP_MT_END;

    memcpy(msg_data, &resp, sizeof(resp));
    if (mq_send(client_mqd, msg_data, sizeof(resp), 0) == -1)
        errMsg("mq_send");
}

int main(void)
{
    /* Create server message queue */

    struct mq_attr attr;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = 10;

    int flags = O_RDWR | O_CREAT | O_EXCL;
    mqd_t server_mqd = mq_open(PMSG_PATH, flags, 0600, &attr);
    if (server_mqd == (mqd_t) -1)
        errExit("mq_open");

    /* Establish SIGCHLD handler to reap terminated children */

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    sa.sa_flags = 0;
    sa.sa_handler = sigInt;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaciton");

    /* Read requests, handle each in a separate child process */

    char msg_data[MSG_SIZE];

    struct request_msg req;

    for (;;) {
        int num_read = mq_receive(server_mqd, msg_data, MSG_SIZE, NULL);
        if (errno == EINTR) {   /* SIGCHILD or SIGKILL Caught */
            if (quit_req)
                break;
            continue;
        }
        if (num_read == -1)
            break;

        pid_t pid = fork();   /* Create child process */
        if (pid == -1) {
            errMsg("fork");
            break;
        }

        if (pid == 0) {   /* Child handles request */
            memcpy(&req, msg_data, num_read);
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    while (1) {
        if (waitpid((pid_t) -1, NULL, 0) == -1) {
            if (errno != ECHILD)
                errMsg("waitpid");
            break;
        }
    }

    /* If fork() or mq_receieve() fails, remove server
     * MQ and exit */

    if (mq_unlink(PMSG_PATH) == -1)
        errExit("mq_unlink");
    exit(EXIT_FAILURE);
}
