#define MAX_MSG_LEN 4096
#define USE_BUF 1
#define USE_SWAP 0

struct rlbuf
{
    int fd;
    int buf_type;
    char *next_char;
    char buf[MAX_MSG_LEN * 2];
    char swap[MAX_MSG_LEN * 2];
};

/* Initialize rlbuf structure */

int read_line_buf_init(int fd, struct rlbuf *rlbuf);

/* Read data from a fd into an rlbuf structure and return
 * the first newline delimited line */

int read_line_buf(struct rlbuf *rlbuf, char *line);
