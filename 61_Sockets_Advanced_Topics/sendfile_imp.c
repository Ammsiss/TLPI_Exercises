#include "tlpi_hdr.h" // IWYU pragma: export
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 1000

ssize_t sendfile_imp(int out_fd, int in_fd, off_t *offset, size_t count)
{
    int num_read;
    char buf[BUF_SIZE];
    ssize_t num_sent = 0;
    int read_cnt;
    off_t initial_offset;

    if (offset) {
        if ((initial_offset = lseek(in_fd, 0, SEEK_CUR)) == -1)
            errExit("lseek");
        if (lseek(in_fd, *offset, SEEK_SET) == -1)
            return -1;
    }

    while (count > 0) {
        read_cnt = count < BUF_SIZE ? count : BUF_SIZE;

        num_read = read(in_fd, buf, read_cnt);
        if (num_read == -1)
            return -1;
        if (num_read == 0)
            break;

        if (write(out_fd, buf, num_read) == -1)
            return -1;

        count -= num_read;
        num_sent += num_read;
    }

    if (offset) {
        if ((*offset = lseek(in_fd, 0, SEEK_CUR)) == -1)
            return -1;
        if (lseek(in_fd, initial_offset, SEEK_SET) == -1)
            return -1;
    }

    return num_sent;
}

int main(int argc, char **argv)
{
    if (argc != 3 || (argc > 1 && strcmp(argv[1], "--help")) == 0)
        usageErr("%s file", argv[0]);

    int in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1)
        errExit("open");

    int out_fd = open(argv[2], O_CREAT | O_EXCL | O_RDWR, 0600);
    if (out_fd == -1)
        errExit("open");

    struct stat sb;
    if (fstat(in_fd, &sb) == -1)
        errExit("fstat");

    off_t offset = 10;

    int num_sent = sendfile_imp(out_fd, in_fd, &offset, sb.st_size);
    if (num_sent == -1)
        errExit("sendfile");

    off_t final_offset;
    if ((final_offset = lseek(in_fd, 0, SEEK_CUR)) == -1)
        errExit("lseek");

    printf("Requested: %ld, Sent: %d, offset = %ld\n", sb.st_size, num_sent, offset);
    printf("in_fd offset after sendfile_imp call: %ld\n", final_offset);
}
