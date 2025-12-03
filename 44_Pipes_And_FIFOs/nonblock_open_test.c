#define _GNU_SOURCE

#include <linux/limits.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#define FIFO_PATH "/tmp/test_fifo"

void callUsageErr(char *program_name)
{
    usageErr("%s [r|w] [o]\n    r: open with O_RDONLY\n"
                            "    w: open with O_WRONLY\n"
                            "    o: other end of FIFO open\n", program_name);
}

void unlinkFifo(void)
{
    if (unlink(FIFO_PATH) == -1)
        errMsg("unlink");
}

int main(int argc, char **argv)
{

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &sa, NULL) == -1)
        errExit("sigaction");

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        callUsageErr(argv[0]);

    if (mkfifo(FIFO_PATH, 0660) == -1)
        errExit("mkfifo");
    if (atexit(unlinkFifo) == -1)
        errExit("atexit");

    char msg[100] = "Attempting to open FIFO with the following setup:\n"
                    "    Flags: O_NONBLOCK, ";

    int open_flags = O_NONBLOCK;
    int dummy_flags = O_NONBLOCK;

    if (argv[1][0] == 'r') {
        open_flags |= O_RDONLY;
        dummy_flags |= O_RDWR;   /* Add read to not trigger ENXIO */
        strcat(msg, "O_RDONLY\n");
    } else if (argv[1][0] == 'w') {
        open_flags |= O_WRONLY;
        dummy_flags |= O_RDONLY;
        strcat(msg, "O_WRONLY\n");
    } else
        callUsageErr(argv[0]);

    strcat(msg, "    Other end open? ");

    int dummy_fd = -1;
    if (argc == 3 && argv[2][0] == 'o') {
        if ((dummy_fd = open(FIFO_PATH, dummy_flags)) == -1)
            errExit("open (dummy)");
        strcat(msg, "Yes");
    } else
        strcat(msg, "No");

    printf("%s\n\n", msg);

    int main_fd;
    if ((main_fd = open(FIFO_PATH, open_flags)) == -1) {
        errMsg("open");
        if (errno == ENXIO)
            fprintf(stderr, "    You get this error becuase if a process attempts a\n"
                            "    write on a FIFO without a reader, the process would\n"
                            "    recieve SIGPIPE and EPIPE.\n");
        exit(EXIT_FAILURE);
    }

    printf("Success!\n\n");

    if (fcntl(main_fd, F_GETPIPE_SZ) != 65536)
        fatal("Must change pipe max size constant!\n");

    if (argv[1][0] == 'r') {
        printf("We will now test nonblocking read operations "
                "with your file descriptor.\n");

        char input[5];

        if (dummy_fd != -1) {
            printf("Keep other end open? [y/n]\n> ");
            if (fgets(input, 2, stdin) == NULL)
                exit(EXIT_SUCCESS);   /* Exit if EOF */
            switch (input[0]) {
            case 'n':
                if (close(dummy_fd) == -1)
                    errExit("close");
                break;
            case 'y':
                break;
            default:
                fatal("Invalid input");
            }
            while (fgetc(stdin) != '\n');
        }

        int to_write;
        printf("Enter number of bytes that should be in the pipe [0,100]\n> ");
        if (fgets(input, 5, stdin) == NULL)
            exit(EXIT_SUCCESS);
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        to_write = getInt(input, GN_NONNEG, "to_write");
        if (to_write > 100)
            fatal("Input must be between 0 and 100");
        /* Open temp fd to get data in pipe */
        int temp_fd = open(FIFO_PATH, O_RDWR | O_NONBLOCK);
        if (temp_fd == -1)
            errExit("open");
        char *temp_buf = malloc(to_write);
        if (temp_buf == NULL)
            errExit("malloc");
        memset(temp_buf, 1, to_write);
        if (write(temp_fd, temp_buf, to_write) != to_write)
            fatal("Failed to write to temp_fd");
        close(temp_fd);

        int to_read;
        printf("Enter number of bytes to be read from the pipe [0,200]\n> ");
        if (fgets(input, 4, stdin) == NULL)
            exit(EXIT_SUCCESS);
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        to_read = getInt(input, GN_NONNEG, "to_write");
        if (to_read > 200)
            fatal("Input must be between 0 and 200");
        printf("\nAttempting read...\n\n");
        char dummy_buf[200];
        int num_read = read(main_fd, dummy_buf, to_read);
        switch (num_read) {
        case 0:
            printf("EOF returned!\n");
            break;
        case -1:
            errMsg("open");
            if (errno == EAGAIN)
                fprintf(stderr, "    This error is becuase you specified O_NONBLOCK\n"
                                "    and the read call would have otherwise blocked.\n");
            exit(EXIT_FAILURE);
        default:
            printf("%d bytes read\n", num_read);
            break;
        }
    } else {
        printf("We will now test nonblocking write operations "
               "with your file descriptor. (PIPE_BUF=%d)\n", PIPE_BUF);

        char input[10];

        if (dummy_fd != -1) {
            printf("Keep other end open? [y/n]\n> ");
            if (fgets(input, 2, stdin) == NULL)
                exit(EXIT_SUCCESS);   /* Exit if EOF */
            switch (input[0]) {
            case 'n':
                if (close(dummy_fd) == -1)
                    errExit("close");
                break;
            case 'y':
                break;
            default:
                fatal("Invalid input");
            }
            while (fgetc(stdin) != '\n');
        }

        int to_write;
        printf("Enter number of bytes that should be in the pipe [0,65536]\n> ");
        if (fgets(input, 7, stdin) == NULL)
            exit(EXIT_SUCCESS);
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        to_write = getInt(input, GN_NONNEG, "to_write");
        if (to_write > 65536)
            fatal("Input must be between 0 and 65536");
        /* Open temp fd to get data in pipe */
        int temp_fd = open(FIFO_PATH, O_RDWR | O_NONBLOCK);
        if (temp_fd == -1)
            errExit("open");
        char *temp_buf = malloc(to_write);
        if (temp_buf == NULL)
            errExit("malloc");
        memset(temp_buf, 1, to_write);
        if (write(temp_fd, temp_buf, to_write) != to_write)
            fatal("Failed to write to temp_fd");
        close(temp_fd);

        printf("Enter number of bytes to be written to the pipe [0,70000]\n> ");
        if (fgets(input, 7, stdin) == NULL)
            exit(EXIT_SUCCESS);
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        to_write = getInt(input, GN_NONNEG, "to_write");
        if (to_write > 70000)
            fatal("Input must be between 0 and 70000");
        printf("\nAttempting write...\n\n");
        char dummy_buf[200];
        memset(dummy_buf, 1, 200);
        int num_written = write(main_fd, dummy_buf, to_write);
        switch (num_written) {
        case -1:
            errMsg("write");
            if (errno == EAGAIN)
                fprintf(stderr, "    This error is becuase you specified an amount\n"
                                "    less then PIPE_BUF but there was not sufficient\n"
                                "    space in the pipe OR you specified an amount\n"
                                "    larger then PIPE_BUF but the pipe was full.\n"
                                "    (See page 918 for more details)\n");
            else if (errno == EPIPE)
                fprintf(stderr, "    This error is becuase you attempted a write with\n"
                                "    the read end closed\n");
            exit(EXIT_FAILURE);
        default:
            printf("%d bytes written\n", num_written);
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
