////////////////////////////////////////////////////////////////////////
// 5.14 - 5-5. Verify shared data between process fd's
////////////////////////////////////////////////////////////////////////

/*
There are a couple ways to verify that duplicated process fd's that point
to the same open file description, share the same file offset and open file
status flags.

In this program I will verify this by using 2 duplicated process fd's to
write to the same file and verify by disabling write access and enabling
read access to read the contents of the file.
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void)
{
    int fd = open("test.txt", O_CREAT | O_EXCL | O_WRONLY, 0600);
    if (fd == -1)
        exit(EXIT_FAILURE);

    int fd2 = dup(fd);
    if (fd2 == -1)
        exit(EXIT_FAILURE);

    // If this prints to the same file, offset is shared.
    if (write(fd, "Hello, ", 7) == -1)
        exit(EXIT_FAILURE);
    if (write(fd2, "world!\n", 7) == -1)
        exit(EXIT_FAILURE);

    // Update fd flags
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        exit(EXIT_FAILURE);

    flags |= O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1)
        exit(EXIT_FAILURE);

    // If flags in fd2 are updated as well it is confirmed.
    int flags2 = fcntl(fd2, F_GETFL);
    if (flags2 == -1)
        exit(EXIT_FAILURE);

    if (flags2 & O_APPEND)
        if (write(STDOUT_FILENO, "Flags are the same!\n", 20) == -1)
            exit(EXIT_FAILURE);
}
