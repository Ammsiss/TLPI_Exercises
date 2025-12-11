#include <fcntl.h>
#include <unistd.h>

#include "tlpi_hdr.h" // IWYU pragma: export
#include "dynamic_array.h"

int read_back(int fd, char* c)
{
    if (read(fd, c, 1) == -1)
        errExit("read");

    return lseek(fd, -2, SEEK_CUR);
}

int main(int argc, char** argv)
{
    if (argc != 2 && argc != 3)
        usageErr("01_ex.bin [ numlines ] file");

    int fileIndex = 1;
    int numLines = 10;
    if (argc == 3) {
        ++fileIndex;

        char *endptr;
        int num = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0')
            usageErr("01_ex.bin [ -n num ] file");

        numLines = num;
    }

    int fd = open(argv[fileIndex], O_RDONLY);
    if (fd == -1) errExit("open");

    DynArray lines;
    da_init(&lines, 0, sizeof(DynArray));

    lseek(fd, -1, SEEK_END);

    for (int i = 0; i <= numLines; ++i) {
        DynArray *line = da_push(&lines);
        da_init(line, 0, 1);

        while (1) {
            char c;
            char *element = da_push(line);

            if (read_back(fd, &c) == -1) {
                *element = c;
                element = da_push(line);
                *element = '\0';
                goto double_break;
            }

            if (c == '\n') {
                *element = '\0';
                break;
            }

            *element = c;
        }
    }
    double_break:

    da_reverse(&lines);

    // Print
    for (size_t i = 0; i < lines.size; ++i) {
        DynArray *line = da_get(&lines, i);
        da_reverse(line);

        if (write(STDOUT_FILENO, line->data, line->size) == 1)
            continue;
        write(STDOUT_FILENO, "\n", 1);
    }

    // Free
    for (size_t i = 0; i < lines.size; ++i)
        da_free((DynArray *)da_get(&lines, i));
    da_free(&lines);
    close(fd);
}
