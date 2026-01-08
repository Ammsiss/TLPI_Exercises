#include "tlpi_hdr.h" // IWYU pragma: export
#include "read_line_buf.h"

#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s msg_len [ msg_len ... ]", argv[0]);

    int fd = open("/tmp/rlbuf_test_file.txt", O_RDWR | O_CREAT, 0600);
    if (fd == -1)
        errExit("open");

    int file_size = 0;
    for (char **carg = &argv[1]; *carg != NULL; ++carg) {
        int msg_size = getInt(*carg, GN_NONNEG, "cmd arg");

        file_size += msg_size + 1;
        if (ftruncate(fd, file_size) == -1)
            errExit("ftruncate");

        for (int i = 0; i < msg_size; ++i)
            if (write(fd, "x", 1) != 1)
                errExit("write");
        if (write(fd, "\n", 1) != 1)
            errExit("write");
    }
    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");

    struct rlbuf rlb;
    if (read_line_buf_init(fd, &rlb) == -1)
        errExit("read_line_buf_init");

    char line[MAX_MSG_LEN * 2];

    int msg_size;
    while ((msg_size = read_line_buf(&rlb, line)) > 0) {
        printf("(%d): %s\n", msg_size, line);
    }
    if (msg_size == -1)
        errExit("read_line_buf");
}
