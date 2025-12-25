#include "tlpi_hdr.h" /* IWYU pragma: export */

#include "pmsg_file.h"

static char client_mq_path[PATH_MAX];

static void removeQueue(void)
{
    if (mq_unlink(client_mq_path) == -1)
        errMsg("mq_unlink");
}

static volatile sig_atomic_t quit_req = 0;

static void sigInt(int sig)
{
    (void) sig;

    quit_req = 1;
}

int main(int argc, char **argv)
{
    struct request_msg req;
    struct response_msg resp;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
        cmdLineErr("pathname too long (max: %ld bytes)\n",
                (long) sizeof(req.pathname) - 1);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigInt;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    struct mq_attr attr;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = 10;

    /* Add path to request struct */
    snprintf(client_mq_path, PATH_MAX, "/client_mq_%d", getpid());
    strcpy(req.client_pmsg, client_mq_path);

    mqd_t client_mqd = mq_open(client_mq_path, O_RDONLY | O_CREAT, 0600, &attr);
    if (client_mqd == -1)
        errExit("mq_open - %s", client_mq_path);

    if (atexit(removeQueue) != 0)
        errExit("atexit");

    /* Add file specified in argv[1] to request struct */
    strncpy(req.pathname, argv[1], sizeof(req.pathname));
    req.pathname[sizeof(req.pathname) - 1] = '\0';

    char *msg_data = malloc(MSG_SIZE);
    if (msg_data == NULL)
        errExit("malloc");

    mqd_t server_mqd = mq_open(PMSG_PATH, O_WRONLY);
    if (server_mqd == (mqd_t) -1)
        errExit("mq_open - PMSG_PATH");

    memcpy(msg_data, &req, sizeof(req));

    if (mq_send(server_mqd, msg_data, sizeof(req), 0) == -1)
        errExit("mq_send");

    /* Message sent; mqd no longer needed */

    if (mq_close(server_mqd) == -1)
        errExit("mq_close");

    int num_msg = 0;
    int tot_bytes = 0;

    do {
        int num_read = mq_receive(client_mqd, msg_data, MSG_SIZE, NULL);
        if (num_read == -1)
            errExit("mq_receive");

        memcpy(&resp, msg_data, num_read);

        if (resp.mtype == RESP_MT_FAILURE) {
            printf("RESP_MT_FAILURE: %s\n", resp.data);   
            exit(EXIT_FAILURE);
        }

        ++num_msg;
        tot_bytes += num_read;

    } while (resp.mtype != RESP_MT_END);

    printf("Received %ld bytes (%d messages)\n", (long) tot_bytes, num_msg);
    exit(EXIT_SUCCESS);
}
