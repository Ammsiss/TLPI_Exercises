/* This program tests the various combinations of MIN and TIME
 * for Non-canonical mode. See 'termios' man page for descriptions */

#define _GNU_SOURCE

#include <ctype.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

static struct termios tb_old;
static volatile sig_atomic_t sig_quit_req = 0;

void restoreTerminal(void)
{
    /* Try to restore terminal settings */
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb_old) == -1) {
        errMsg("tcsetattr");
        printf("***FAILED TO RESTORE TERMINAL SETTINGS***\n");
    }
}

void intr_hand(int sig)
{
    (void) sig;
    sig_quit_req = 1;
}

void interByte(int min, int time)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = intr_hand;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    struct termios tb;
    if (tcgetattr(STDIN_FILENO, &tb) == -1)
        errExit("tcgetattr");

    tb.c_lflag &= ~ICANON;
    tb.c_lflag &= ~ECHO;
    for (int i = 0; i < NCCS; ++i)
        tb.c_cc[i] = _POSIX_VDISABLE;
    tb.c_cc[VINTR] = 003;
    tb.c_cc[VMIN] = min;
    tb.c_cc[VTIME] = time;

    printf("Starting Non-canonical mode; interbyte timeout (MIN=%d, TIME=%d)\n\n", min, time);
    printf("    This mode is the same as read with timeout except that the\n");
    printf("    timer is only started once 1 byte has been made available and\n");
    printf("    the timer is restarted after every new byte is read.\n\n");

    printf("    To show the use of this mode try using the arrow keys with various\n");
    printf("    different interbyte timer values. Then for each one try and\n");
    printf("    trick the system into thinking that what you manually type is\n");
    printf("    also the corresponding arrow key. This will be much easier with\n");
    printf("    a larger TIME value. This program will only test for escape sequ-\n");
    printf("    ences if you set MIN to 3 (The byte combo for an arrow key can be man-\n");
    printf("    ually typed via (^V<Esc>[D)");
    printf("\n\n(Enter '^C' to exit)\n\n");

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb) == -1)
        errExit("tcsetattr");

    char *buf = malloc(min + 1);
    if (buf == NULL)
        errExit("malloc");
    buf[min] = '\0';

    int num_read;
    while (!sig_quit_req) {
        switch (num_read = read(STDIN_FILENO, buf, min)) {
        case -1:
            if (errno == EINTR)
                continue;
            free(buf);
            errExit("read");
        case 0:
            continue;
        default:
            if (min == 3 && num_read == 3)
                printf("Interbyte timer did not expire. Interpretting as escape sequence.\n");
            else
                printf("Interbyte timer expired. Wont interpret as escape sequence.\n");

            printf("Input read: ");
            for (int i = 0; i < num_read; ++i) {
                if (isalpha((unsigned char) buf[i]))
                    putchar(buf[i]);
                else if (iscntrl((unsigned char) buf[i]))
                    printf("^%c", buf[i] ^ 64);
                else
                    putchar('*');
            }
            printf("\n");
            break;
        }
    }

    free(buf);
}

void readTimeout(int time)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = intr_hand;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    struct termios tb;
    if (tcgetattr(STDIN_FILENO, &tb) == -1)
        errExit("tcgetattr");

    tb.c_lflag &= ~ICANON;
    tb.c_lflag &= ~ECHO;
    for (int i = 0; i < NCCS; ++i)
        tb.c_cc[i] = _POSIX_VDISABLE;
    tb.c_cc[VINTR] = 003;
    tb.c_cc[VMIN] = 0;
    tb.c_cc[VTIME] = time;

    printf("Starting Non-canonical mode; read with timeout (MIN=0, TIME=%d)\n\n", time);
    printf("    In this mode read() immediately returns once 1 byte of data\n");
    printf("    is available or TIME tenths of a second has passed.");
    printf("\n\n(Enter 'q' to exit)\n\n");

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb) == -1)
        errExit("tcsetattr");

    char c;
    while (!sig_quit_req) {
        switch (read(STDIN_FILENO, &c, 1)) {
        case -1:
            if (errno == EINTR)
                continue;
            errExit("read");
        case 1:
            printf("Input read: ");
            fflush(stdout);

            if (isalpha((unsigned char) c))
                putchar(c);
            else if (iscntrl((unsigned char) c))
                printf("^%c", c ^ 64);
            else
                putchar('*');
            printf("\n");

            if (c == 'q')
                sig_quit_req = 1;
            break;
        case 0:   /* Time up */
            printf("read timed out\n");
            continue;
        default:
            errExit("Impossible case resolution.\n");
        }
    }
}

void blockingRead(int min)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = intr_hand;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    struct termios tb;
    if (tcgetattr(STDIN_FILENO, &tb) == -1)
        errExit("tcgetattr");

    tb.c_lflag &= ~ICANON;
    tb.c_lflag &= ~ECHO;
    for (int i = 0; i < NCCS; ++i)
        tb.c_cc[i] = _POSIX_VDISABLE;
    tb.c_cc[VINTR] = 003;
    tb.c_cc[VMIN] = min;
    tb.c_cc[VTIME] = 0;

    printf("Starting Non-canonical mode; blocking read (MIN=%d, TIME=0)\n\n", min);
    printf("    In this mode read() blocks until the lesser of the number of\n");
    printf("    bytes requested or MIN bytes are available.\n\n('^C' to exit)\n\n");

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb) == -1)
        errExit("tcsetattr");

    char *buf = malloc(min + 1);
    if (buf == NULL)
        errExit("malloc");
    buf[min] = '\0';

    int num_read;
    while (!sig_quit_req) {
        switch (num_read = read(STDIN_FILENO, buf, min)) {
        case -1:
            if (errno == EINTR)
                continue;
            free(buf);
            errExit("read");
        default:
            printf("Input read: ");
            fflush(stdout);
            for (int i = 0; i < num_read; ++i) {
                if (isalpha((unsigned char) buf[i]))
                    putchar(buf[i]);
                else if (iscntrl((unsigned char) buf[i]))
                    printf("^%c", buf[i] ^ 64);
                else
                    putchar('*');
            }
            printf("\n");
            break;
        }
    }

    free(buf);
}

void pollingRead()
{
    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)
        errExit("sigprocmask");

    struct timespec ts;
    ts.tv_nsec = 33333333;   /* ~33ms->30fps which will effectively be our polling rate */
    ts.tv_sec = 0;

    struct termios tb;
    if (tcgetattr(STDIN_FILENO, &tb) == -1)
        errExit("tcgetattr");

    tb.c_lflag &= ~ICANON;
    tb.c_lflag &= ~ECHO;
    for (int i = 0; i < NCCS; ++i)
        tb.c_cc[i] = _POSIX_VDISABLE;
    tb.c_cc[VINTR] = 003;
    tb.c_cc[VMIN] = 0;
    tb.c_cc[VTIME] = 0;

    printf("Starting Non-canonical mode; polling read (MIN=0, TIME=0)\n\n");
    printf("    In this mode read() immediately returns 0 if no data is\n");
    printf("    available. We use sigtimedwait() to not burn needless cpu\n");
    printf("    as well as handle the SIGINT signal. \n\n(Enter 'q' to exit)\n\n");

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tb) == -1)
        errExit("tcsetattr");

    char c;
    int quit_request = 0;
    while (!quit_request) {
        switch (read(STDIN_FILENO, &c, 1)) {
        case -1:
            errExit("read");
        case 1:
            printf("Input read: ");
            fflush(stdout);

            if (isalpha((unsigned char) c))
                putchar(c);
            else if (iscntrl((unsigned char) c))
                printf("^%c", c ^ 64);
            else
                putchar('*');
            printf("\n");

            if (c == 'q')
                quit_request = 1;
            break;
        case 0:
            errno = 0;
            if (sigtimedwait(&block_set, NULL, &ts) == -1 && errno != EAGAIN)
                errExit("sigtimedwait");
            if (errno == 0)
                quit_request = 1;
            break;
        default:
            errExit("Impossible case resolution.\n");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s MIN TIME", argv[0]);

    int min = getInt(argv[1], GN_NONNEG, 0);
    int time = getInt(argv[2], GN_NONNEG, 0);

    /* This program just handles SIGINT, but a robust program that
     * modifies terminal state should probably set up a handler for
     * every signal that either modifies the terminal or can be su-
     * spended. (Especially for job control support with SIGTSTP) */

    if (tcgetattr(STDIN_FILENO, &tb_old) == -1)
        errExit("tcgetattr");
    if (atexit(restoreTerminal) == -1)
        errExit("atexit");

    if (time == 0) {
        if (min == 0)
            pollingRead();
        else
            blockingRead(min);
    } else {
        if (min == 0)
            readTimeout(time);
        else
            interByte(min, time);
    }

    return EXIT_SUCCESS;
}
