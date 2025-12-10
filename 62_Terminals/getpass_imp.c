#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

char *getpass_imp(const char *prompt)
{
    static char string_buf[4096];

    int cterm_fd = open("/dev/tty", O_RDONLY);
    if (cterm_fd == -1)
        errExit("open");

    int saved_errno;

    struct termios tb_old;   /* Save a copy so we can restore */
    if (tcgetattr(cterm_fd, &tb_old) == -1) {
        saved_errno = errno;
        if (close(cterm_fd) == -1)   /* Cleanup is best-effort */
            errno = saved_errno;
        return NULL;
    }

    struct termios tb;
    if (tcgetattr(cterm_fd, &tb) == -1)
        return NULL;

    if (tb.c_lflag & ECHO)   /* Turn of input echo */
        tb.c_lflag &= ~ECHO;
    for (int i = 0; i < NCCS; ++i)   /* Turn off all special char processing */
        tb.c_cc[i] = _POSIX_VDISABLE;

    if (tcsetattr(cterm_fd, TCSANOW, &tb) == -1) {
        saved_errno = errno;
        if (close(cterm_fd) == -1)
            errno = saved_errno;
        return NULL;
    }

    printf("%s", prompt);
    fgets(string_buf, 4096, stdin);
    if (string_buf[strlen(string_buf) - 1] == '\n')
        string_buf[strlen(string_buf) - 1] = '\0';

    /* Restore terminal settings */
    if (tcsetattr(cterm_fd, TCSANOW, &tb_old) == -1) {
        saved_errno = errno;
        if (close(cterm_fd) == -1)
            errno = saved_errno;
        return NULL;
    }

    return string_buf;
}


int main(void)
{
    char *name = getpass_imp("Password: ");
    if (name == NULL)
        errExit("getpass_imp");

    printf("\nYou entered: %s\n", name);
}
