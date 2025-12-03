#include <unistd.h>

#include "fifo_seqnum.h"
#include "../lib/error_functions.h"
#include "../lib/utils.h"

static char client_fifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void)
{
    if (unlink(client_fifo) == -1)
        errMsg("unlink");
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len]\n", argv[0]);

    snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
        (long) getpid());

    umask(0);
    if (mkfifo(client_fifo, 0620) == -1 && errno != EEXIST)
        errExit("mkfifo %s", client_fifo);
    if (atexit(removeFifo) != 0)
        errExit("atexit");

    struct request req;
    req.pid = getpid();
    req.seqLen = (argc > 1) ? getNum(argv[1], "Arg must be an integer") : 1;

    int server_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fd == -1)
        errExit("open %s", SERVER_FIFO);

    if (write(server_fd, &req, sizeof(struct request)) !=
        sizeof(struct request))
            fatal("Can't write to server");

    int client_fd = open(client_fifo, O_RDONLY);
    if (client_fd == -1)
        errExit("open %s", client_fifo);

    struct response resp;
    if (read(client_fd, &resp, sizeof(struct response))
        != sizeof(struct response))
        fatal("Can't read response from server");

    printf("%d\n", resp.seqNum);
    exit(EXIT_SUCCESS);
}
