/*
Name:
    imp_tee

Synopsis:
    imp_tee [OPTIONS] [file]

    -a, Append to file if it already exists

Description:
    This program reads from its standard input and writes it to
    stdout as well as a file named in its command line arguments.

    This program was created as a solution to TLPI exersise 4-1.
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char** argv)
{
    int fd = -1;
    bool noFile;

    int openFlags = O_WRONLY;
    switch (argc)
    {
    case 1:
        noFile = true;
        break;
    case 2:
        noFile = false;
        openFlags |= O_CREAT | O_TRUNC;

        fd = open(argv[1], openFlags, 0600);
        if (fd == -1)
            exit(EXIT_FAILURE);

        break;
    case 3:
        if (strcmp(argv[1], "-a") != 0)
            fprintf(stderr, "%s [-a] [file]\n", argv[0]);

        noFile = false;
        openFlags |= O_CREAT | O_APPEND;

        fd = open(argv[2], openFlags, 0600);
        if (fd == -1)
            exit(EXIT_FAILURE);

        break;
    default:
        fprintf(stderr, "%s [OPTIONS] [file]\n", argv[0]);
        break;
    }

    while (true)
    {
        char buf[BUF_SIZE];
        int numRead = read(STDIN_FILENO, buf, BUF_SIZE);

        if (numRead == -1)
            exit(EXIT_FAILURE);

        if (numRead == 0)
        {
            if (write(STDOUT_FILENO, "EOF reached!\n", 13) == -1)
                exit(EXIT_FAILURE);

            break;
        }

        if (write(STDOUT_FILENO, buf, numRead) == -1)
            exit(EXIT_FAILURE);

        if (!noFile)
            if (write(fd, buf, numRead) == -1)
                exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
