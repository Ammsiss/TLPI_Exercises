#include "read_line_buf.h"

#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int read_line_buf_init(int fd, struct rlbuf *rlb)
{
    rlb->buf[0] = '\0';
    rlb->next_char = rlb->buf;
    rlb->fd = fd;
    rlb->buf_type = USE_BUF;

    return 0;
}

int read_line_buf(struct rlbuf *rlbuf, char *line)
{
    char *buf;

    // Figure out which buffer to use
    if (rlbuf->buf_type == USE_BUF)
        buf = rlbuf->buf;
    else if (rlbuf->buf_type == USE_SWAP)
        buf = rlbuf->swap;
    else {
        errno = EINVAL;
        return -1;
    }

    if (*rlbuf->next_char == '\0') {

        int num_read = read(rlbuf->fd, rlbuf->next_char, MAX_MSG_LEN);
        if (num_read < 1)
            return num_read;

        rlbuf->next_char[num_read] = '\0';
        rlbuf->next_char = &buf[0];
    }

    char *msg_start = rlbuf->next_char;

    int msg_len = 0;
    while (1) {
        if (*rlbuf->next_char == '\0') {

            // If message is truncated
            if (*(rlbuf->next_char - 1) != '\n') {

                // Swap the buffer to use
                if (rlbuf->buf_type == USE_BUF) {
                    rlbuf->buf_type = USE_SWAP;
                    buf = rlbuf->swap;
                } else if (rlbuf->buf_type == USE_SWAP) {
                    rlbuf->buf_type = USE_BUF;
                    buf = rlbuf->buf;
                } else {
                    errno = EINVAL;
                    return -1;
                }

                // Copy initial truncated portion into swap buffer
                memcpy(buf, msg_start, msg_len);

                // Null terminate the buffer
                buf[msg_len] = '\0';

                // next_char pointing at the terminating \0
                rlbuf->next_char = &buf[msg_len];

                // Recurse
                return read_line_buf(rlbuf, line);

            } else {
                // Msg finished; copy to user buf
                if (msg_len == 0)
                    return 0;
                memcpy(line, msg_start, msg_len);
                line[msg_len] = '\0';

                // Now reset buf for next message
                buf[0] = '\0';
                rlbuf->next_char = &buf[0];

                break;
            }
        }

        if (*rlbuf->next_char == '\n') {
            ++msg_len;
            memcpy(line, msg_start, msg_len);
            line[msg_len] = '\0';

            ++rlbuf->next_char;
            if (*rlbuf->next_char == '\0') {
                rlbuf->next_char = buf;
                *rlbuf->next_char = '\0';
            }

            break;
        }

        ++msg_len;
        ++rlbuf->next_char;
    }

    return msg_len;
}
