////////////////////////////////////////////////////////////////////////
// 4.10 - 4-2. CP CLONE MAINTAINS FILE HOLES
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUF_SIZE 1024

int main(int argc, char** argv)
{
    if (argc != 3)
        fprintf(stderr, "%s file file\n", argv[0]);

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1)
        exit(EXIT_FAILURE);

    int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0600);
    if (fd2 == -1)
        exit(EXIT_FAILURE);


    char buf[BUF_SIZE];
    while (true)
    {
        // FIGURE OUT DATA / HOLE SIZES
        int numRead;

        int currentOffset = lseek(fd1, 0, SEEK_CUR);
        if (currentOffset == -1)
            exit(EXIT_FAILURE);

        int dataOffset = lseek(fd1, currentOffset, SEEK_DATA);
        if (dataOffset == -1)
            break; // NO MORE DATA

        if (currentOffset == dataOffset)
        {
            int holeOffset = lseek(fd1, currentOffset, SEEK_HOLE);
            if (holeOffset == -1)
                exit(EXIT_FAILURE);

            if (lseek(fd1, currentOffset, SEEK_SET) == -1)
                exit(EXIT_FAILURE);

            numRead = holeOffset - currentOffset;
        }
        else
        {
            if (lseek(fd2, dataOffset, SEEK_SET) == -1)
                exit(EXIT_FAILURE);

            continue;
        }

        // WRITE DATA
        int rv;

        while (numRead > BUF_SIZE)
        {
            rv = read(fd1, buf, BUF_SIZE);
            if (rv == -1)
                exit(EXIT_FAILURE);

            numRead -= BUF_SIZE;

            rv = write(fd2, buf, rv);
            if (rv == -1)
                exit(EXIT_FAILURE);
        }

        rv = read(fd1, buf, numRead);
        if (rv == -1)
            exit(EXIT_FAILURE);

        rv = write(fd2, buf, rv);
        if (rv == -1)
            exit(EXIT_FAILURE);
    }

    if (close(fd1) == -1)
        exit(EXIT_FAILURE);

    if (close(fd2) == -1)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}
