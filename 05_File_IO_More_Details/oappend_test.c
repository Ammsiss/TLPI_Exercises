#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
    int fd = open("test.txt", O_WRONLY | O_APPEND);

    char buf[] = "!data!";
    lseek(fd, 0, SEEK_SET);

    write(fd, buf, 6);
}
