#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../lib/error_functions.h"

#define BUF_SIZE 100

int main(void)
{
    int ptoc[2];
    if (pipe(ptoc) == -1)
        errExit("pipe");
    int ctop[2];
    if (pipe(ctop) == -1)
        errExit("pipe");

    switch (fork()) {
    case -1:
        errExit("fork");
    case 0:
    {
        if (close(ptoc[1]) == -1)
            errExit("close");
        if (close(ctop[0]) == -1)
            errExit("close");

        char c_io_buf[BUF_SIZE];

        while (1) {
            if (read(ptoc[0], c_io_buf, BUF_SIZE) == 0)
                break;
            for (char *c = c_io_buf; *c != '\0'; ++c)
                *c = toupper(*c);
            if (write(ctop[1], c_io_buf, strlen(c_io_buf) + 1) != (int) strlen(c_io_buf) + 1)
                errExit("Error writing to ctop");
        }

        if (close(ptoc[0]) == -1)
            errExit("close");
        if (close(ctop[1]) == -1)
            errExit("close");

        exit(EXIT_SUCCESS);
    }
    default:
        break;
    }

    if (close(ptoc[0]) == -1)
        errExit("close");
    if (close(ctop[1]) == -1)
        errExit("close");

    char p_io_buf[BUF_SIZE];

    while (1) {
        if (fgets(p_io_buf, BUF_SIZE, stdin) == NULL)
            break;   /* Break on EOF */

        /* Remove trailing newline */
        if (p_io_buf[strlen(p_io_buf) - 1] == '\n')
            p_io_buf[strlen(p_io_buf) - 1] = '\0';

        int mes_len = strlen(p_io_buf) + 1;

        /* Error on partial reads or writes */
        if (write(ptoc[1], p_io_buf, mes_len) != mes_len)
            errExit("Error writing to ptoc");
        if (read(ctop[0], p_io_buf, mes_len) != mes_len)
            errExit("Error reading from ctop");

        printf("%s\n", p_io_buf);
    }

    if (close(ptoc[1]) == -1)
        errExit("close");
    if (close(ctop[0]) == -1)
        errExit("close");

    if (waitpid(-1, NULL, 0) == -1)
        errExit("waitpid");

    exit(EXIT_SUCCESS);
}
