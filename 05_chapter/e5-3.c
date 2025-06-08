////////////////////////////////////////////////////////////////////////
// 5.14 - 5-3. Why atomicity with O_APPEND is necessary.
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
    if (argc != 3 && argc != 4)
        fprintf(stderr, "atomic_append filename num-bytes [x]1\n");

    bool noAppend = false;
    if (argc == 4)
    {
        if (strcmp(argv[3], "x") != 0)
            fprintf(stderr, "atomic_append filename num-bytes [x]2\n");

        noAppend = true;
    }

    int flags = O_WRONLY | O_CREAT;
    if (!noAppend)
        flags |= O_APPEND;

    int fd = open(argv[1], flags, 0600);
    if (fd == -1)
        exit(EXIT_FAILURE);

    char* errPointer;

    int numBytes = strtol(argv[2], &errPointer, 10);
    if (*errPointer != '\0')
        fprintf(stderr, "atomic_append filename num-bytes [x]3\n");

    for (int x = 0; x < numBytes; ++x)
    {
        if (noAppend)
            if (lseek(fd, 0, SEEK_END) == -1)
                exit(EXIT_FAILURE);

        if (write(fd, "X", 1) == -1)
            exit(EXIT_FAILURE);
    }

    if (close(fd) == -1)
        exit(EXIT_FAILURE);

    return EXIT_SUCCESS;
}
