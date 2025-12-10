#include <termios.h>
#include <unistd.h>

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

/* Define to test other control flow */
#define NONCANON

int main(void)
{
#ifdef NONCANON
    struct termios tb_old;
    if (tcgetattr(STDIN_FILENO, &tb_old) == -1)
        errExit("tcgetattr");
#endif
    struct termios tb;
    if (tcgetattr(STDIN_FILENO, &tb) == -1)
        errExit("tcgetattr");
#ifdef NONCANON
    if (tb.c_lflag & ICANON)
        tb.c_lflag &= ~ICANON;
    if (tb.c_lflag & ECHO)
        tb.c_lflag &= ~ECHO;

    for (int i = 0; i < NCCS; ++i)
        tb.c_cc[i] = _POSIX_VDISABLE;
    tb.c_cc[VMIN] = 1;   /* Set up blocking read */
    tb.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb) == -1)
        errExit("tcgetattr");
#endif
    if (tb.c_lflag & ICANON)
        printf("You are in canonical mode.\n");
    else {
        printf("You are not in canonical mode (");
        printf("TIME=%d MIN=%d)\n", tb.c_cc[VTIME], tb.c_cc[VMIN]);

        printf("\nTry typing to see the difference ('q' to exit)\n> ");
        fflush(stdout);

        char c;
        while (1) {
            if (read(STDIN_FILENO, &c, 1) == -1) {
                errMsg("read");
                break;
            }
            if (c == 'q') {
                printf("\nBye!\n");
                break;
            }
            if (write(STDOUT_FILENO, &c, 1) == -1) {
                errMsg("write");
                break;
            }
        }
    }
#ifdef NONCANON
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb_old) == -1)
        errExit("tcgetattr");
#endif

    exit(EXIT_SUCCESS);
}
