#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define _FILE_OFFSET_BITS 64

int
main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        fprintf(stderr, "%s pathname offset\n", argv[0]);

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        exit(EXIT_FAILURE);

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1)
        exit(EXIT_FAILURE);

    if (write(fd, "test", 4) == -1)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}

// Deprecated method.

// #define _LARGEFILE64_SOURCE
//
// int
// main(int argc, char *argv[])
// {
//     int fd;
//     off64_t off;
//
//     if (argc != 3 || strcmp(argv[1], "--help") == 0)
//         fprintf(stderr, "%s pathname offset\n", argv[0]);
//
//     fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
//     if (fd == -1)
//         exit(EXIT_FAILURE);
//
//     off = atoll(argv[2]);
//     if (lseek64(fd, off, SEEK_SET) == -1)
//         exit(EXIT_FAILURE);
//
//     if (write(fd, "test", 4) == -1)
//         exit(EXIT_FAILURE);
//
//     exit(EXIT_SUCCESS);
// }
