#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Well-known name for server's FIFO */
#define SERVER_FIFO "/tmp/seqnum_sv"

/* Template for building client FIFO name */
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"

/* Space required for client FIFO pathname
(+20 as a generous allowance for the PID) */
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request {   /* Request (client --> server) */
    pid_t pid;     /* PID of client */
    int seqLen;    /* Length of desired sequence */
};

struct response {  /* Response (server --> client) */
    int seqNum;    /* Start of sequence */
};
