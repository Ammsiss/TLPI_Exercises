#define _GNU_SOURCE

#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "../lib/error_functions.h"
#include "../lib/utils.h"
#include "fifo_seqnum.h"

#define MAX_DIGITS 9         /* Max 9 digits becuase thats the max an int can store */
#define MAX_SIZE 999999999   /* 9 digit max number */

#define REQ_LEN sizeof(struct request)
#define RES_LEN sizeof(struct response)

#define NO_EINTR(stmt) while ((stmt) == -1 && errno == EINTR);

static volatile sig_atomic_t stop_requested = 0;

/* Code relating to signal handlers; exersize 44-4 */
/* ------------------------------------------- */
void setDeathFlag(int sig)
{
    (void) sig;
    stop_requested = 1;
}
/* ------------------------------------------- */

void unlinkFifo(void)
{
    if (unlink(SERVER_FIFO) == -1)
        errMsg("unlink");
}

int main(void)
{
/* Code relating to signal handlers; exersize 44-4
        (Also moved SIGPIPE handling here) */
/* ------------------------------------------- */
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = setDeathFlag;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGTERM, &sa, NULL) == -1)
        errExit("sigaction");
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &sa, NULL) == -1)  /* Ignore SIGPIPE to get informed of bad */
        errExit("sigaction");                 /* client FIFO's via failed write() */
/* ------------------------------------------- */

    int seq_num = 0;   /* Our "service" */

/* Code relating to back up file; exersize 44-3 */
/* ------------------------------------------- */
    char seq_num_buf[MAX_DIGITS + 1];

    int backup_fd = open("./seqnum_backup.txt", O_CREAT | O_RDWR, 0660);
    if (backup_fd == -1)
        errExit("open");

    int num_read = read(backup_fd, seq_num_buf, MAX_DIGITS);
    if (num_read == -1)
        errExit("read");

    if (num_read != 0) {   /* If not EOF then file exists */
        seq_num_buf[num_read] = '\0';
        seq_num = getNum(seq_num_buf, "getNum");
    }
/* ------------------------------------------- */

    umask(0);                /* Error on everything besides EEXIST */
    if (mkfifo(SERVER_FIFO, 0660) == -1 && errno != EEXIST)
        errExit("mkfifo %s", SERVER_FIFO);
    if (atexit(unlinkFifo) != 0)   /* Set up handler to unlink FIFO */
        errExit("atexit");

    int server_fd = open(SERVER_FIFO, O_RDONLY | O_NONBLOCK);
    if (server_fd == -1)
        errExit("open %s", SERVER_FIFO);

    /* Turn off O_NONBLOCK; only needed it for opening both ends of FIFO */

    int flags = fcntl(server_fd, F_GETFL);
    if (flags == -1)
        errExit("fcntl");
    flags &= ~O_NONBLOCK;
    if (fcntl(server_fd, F_SETFL, flags) == -1)
        errExit("fcntl");

    /* Open an extra write descriptor so that we never see EOF on read */
    if (open(SERVER_FIFO, O_WRONLY) == -1)
        errExit("open %s", SERVER_FIFO);

    char client_fifo[CLIENT_FIFO_NAME_LEN];
    struct request client_req;
    struct response serv_resp;

    int rv;

    while (1) {
/* Code relating to signal handlers; exersize 44-4 */
/* ------------------------------------------- */
        if (stop_requested)
            break;
/* ------------------------------------------- */

        errno = 0;
        if (read(server_fd, &client_req, REQ_LEN) != REQ_LEN) {
            if (errno != EINTR)
                fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }

        snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
            (long) client_req.pid);

        /* Open client FIFO for reading just so that we can open it
         * for writing */

/* Code relating to handling malicious client; exersize 44-5 */
/* ------------------------------------------- */
        int dummyFd;
        NO_EINTR(dummyFd = open(client_fifo, O_RDONLY | O_NONBLOCK));
        if (dummyFd == -1) {
            errMsg("openn %s", client_fifo);
            continue;
        }
/* ------------------------------------------- */

        int client_fd;
        NO_EINTR(client_fd = open(client_fifo, O_WRONLY));
        if (client_fd == -1) {
            close(dummyFd);
            errMsg("open %s", client_fifo);
            continue;
        }

/* Code relating to handling malicious client; exersize 44-5 */
/* ------------------------------------------- */
        close(dummyFd);   /* Close this so that we can still get EPIPE */
/* ------------------------------------------- */

        serv_resp.seqNum = seq_num;

/* Code relating to handling malicious client; exersize 44-5 */
/* ------------------------------------------- */
        int retry_count = 1;
        while (retry_count < 4) {
            errno = 0;
            NO_EINTR(rv = write(client_fd, &serv_resp, RES_LEN));
            if (errno != EPIPE)
                break;
            fprintf(stderr, "Failed to write to %s\n", client_fifo);
            fprintf(stderr, "    Trying again...(Attmpt %d/3)\n", retry_count);
            sleep(1);
            ++retry_count;
        }
        if (rv != RES_LEN)
            fprintf(stderr, "Error writing to FIFO %s\n", client_fifo);
/* ------------------------------------------- */

        NO_EINTR(rv = close(client_fd));
        if (rv == -1)
            errMsg("close");

        if ((long) client_req.seqLen + seq_num > MAX_SIZE ||
                (long) client_req.seqLen + seq_num < -MAX_SIZE)
            fprintf(stderr, "Requested number too large; no effect\n");
        else
            seq_num += client_req.seqLen;

/* Code relating to back up file; exersize 44-3 */
/* ------------------------------------------- */
        NO_EINTR(rv = ftruncate(backup_fd, 0));
        if (rv == -1)
            errExit("ftruncate");
        if (lseek(backup_fd, 0, SEEK_SET) == -1)
            errExit("lseek");
        snprintf(seq_num_buf, 10, "%d", seq_num);
        NO_EINTR(rv = write(backup_fd, seq_num_buf, strlen(seq_num_buf)));
        if (rv == -1)
            errExit("write");
/* ------------------------------------------- */
    }

    exit(EXIT_SUCCESS);
}
