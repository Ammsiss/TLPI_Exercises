#include "tlpi_hdr.h" // IWYU pragma: export

#include <sys/random.h>
#include <sys/time.h>
#include <time.h>
#include <termios.h>

#define BUF_SIZE 256

/* Program to be used with unbuffer.c */

int main(void) {
    char buf[256];

    /* 100 million ns; 1/10 of a sec */
    struct timespec ts = { 0, 100000000 };

    int num_read;
    int loop_count = 0;
    while (1) {
        if (loop_count % 5 == 0) {
            if ((num_read = read(STDIN_FILENO, buf, 256)) == -1)
                errExit("read");
            buf[num_read - 1] = '\0';
        }

        printf("%s\n", buf);   /* Random num [0,100] */
        if (clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL) == -1)
            errExit("clock_nanosleep");
        ++loop_count;
    }
}
