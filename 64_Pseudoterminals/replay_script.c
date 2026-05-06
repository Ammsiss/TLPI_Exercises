#include "tlpi_hdr.h" // IWYU pragma: export

#include <string.h>
#include <fcntl.h>
#include <time.h>

#include "error_functions.h"
#include "dynamic_array.h"

int main(int argc, char **argv) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s <file>.timed", argv[0]);

    int tscriptFd = open(argv[1], O_RDONLY, 0600);
    if (tscriptFd == -1)
        errExit("fd");

    bool eof = false;
    int num_read;

    int last_ms = 0;

    while (1) {
        char ms[100] = "";
        char c;

        while (1) {
            num_read = read(tscriptFd, &c, 1);
            if (num_read == 0) {
                eof = true;
                break;
            } else if (num_read == -1)
                errExit("read");

            if (c == ' ')
                break;

            ms[strlen(ms)] = c;
        }

        if (eof)
            break;

        ms[strlen(ms)] = '\0';
        int current_ms = getInt(ms, GN_NONNEG, "nsec");
        int miliseconds = current_ms - last_ms;
        last_ms = current_ms;

        struct timespec ts;
        ts.tv_sec = miliseconds / 1000;
        ts.tv_nsec = (miliseconds % 1000) * 1000000;

        if (clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL) != 0)
            errExit("clock_nanosleep");

        DynArray string;
        da_init(&string, 0, 1);

        while (1) {
            num_read = read(tscriptFd, &c, 1);
            if (num_read == 0) {
                eof = true;
                break;
            } else if (num_read == -1)
                errExit("read");

            if (c == '\n')
                break;
            else if (c == 'n') {
                char *last = da_get(&string, string.size - 1);
                if (*last == '\\') {
                    *last = '\n';
                    continue;
                }
            }

            char *element = da_push(&string);
            if (!element)
                errExit("da_push");

            *element = c;
        }

        for (size_t i = 0; i < string.size; ++i) {
            char *element = da_get(&string, i);
            if (element == NULL)
                errExit("da_get");
            printf("%c", *element);
        }
        fflush(stdout);

        da_free(&string);

        if (eof)
            break;
    }
}
