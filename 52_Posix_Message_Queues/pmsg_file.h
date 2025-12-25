#define _GNU_SOURCE

#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PMSG_PATH "/pmsg_file_server"   /*  Well-known server message queue */

struct request_msg {              /* Requests (client to server) */
    char client_pmsg[PATH_MAX];   /* Path of client's message queue */
    char    pathname[PATH_MAX];   /* File to be returned */
};

struct response_msg {   /* Responses (server to client) */
    long mtype;         /* One of RESP_MT_* values below */
    char data[4096];    /* File content / response message */
};

#define MSG_SIZE 8192   /* Default msgsize max */

/* Types for response messages sent from server to client */

#define RESP_MT_FAILURE 1   /* File couldn't be opened */
#define RESP_MT_DATA    2   /* Message contains file data */
#define RESP_MT_END     3   /* File data complete */
