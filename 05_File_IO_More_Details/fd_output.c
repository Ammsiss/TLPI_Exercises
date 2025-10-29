#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
    // Open test.txt, creating if it doesn't exist, for reading and writing
    // truncating existing content, with permissions equal to 0600.
    int fd1 = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    // Duplicates the existing process fd, both point to the same open file
    // discription.
    int fd2 = dup(fd1);

    // Open test.txt, for reading and writing. Creates a new open file
    // discription.
    int fd3 = open("test.txt", O_RDWR);

    // Writes to test.txt pushing the open fd offset to position 6.
    write(fd1, "Hello,", 6);

    // Writes to test.txt from offset 6 - 11, offset ends at 12.
    write(fd2, "world", 6);

    // Resets fd1, and fd2's open file discription offset to 0.
    lseek(fd2, 0, SEEK_SET);

    // overwrites the existing "hello," with "HELLO,"
    write(fd1, "HELLO,", 6);

    // overwrites "HELLO," with "Gidday".
    write(fd3, "Gidday", 6);

    // Final file data: "Giddayworld".
}
