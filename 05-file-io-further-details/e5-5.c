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

// int main(void)
// {
//     int fd = open("test.txt", O_CREAT | O_EXCL | O_WRONLY, 0600);
//     if (fd == -1) errExit("open");
//
//     int fd2 = dup(fd);
//     if (fd2 == -1) errExit("dup");
//
//     // If this prints to the same file, offset is shared.
//     if (write(fd, "Hello, ", 7) == -1) errExit("write");
//     if (write(fd2, "world!\n", 7) == -1) errExit("write");
//
//     // Update fd flags
//     int flags = fcntl(fd, F_GETFL);
//     if (flags == -1) errExit("fcntl");
//
//     flags |= O_APPEND;
//     if (fcntl(fd, F_SETFL, flags) == -1) errExit("fcntl");
//
//     // If flags in fd2 are updated as well it is confirmed.
//     int flags2 = fcntl(fd2, F_GETFL);
//     if (flags2 == -1) errExit("fcntl");
//
//     if (flags2 & O_APPEND)
//         if (write(STDOUT_FILENO, "Flags are the same!\n", 20) == -1)
//             errExit("write");
// }
