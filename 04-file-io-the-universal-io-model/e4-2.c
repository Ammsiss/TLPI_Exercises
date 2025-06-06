#define _GNU_SOURCE

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

////////////////////////////////////////////////////////////////////////
// 4.10 - 4-2. CP CLONE MAINTAINS FILE HOLES
////////////////////////////////////////////////////////////////////////

// #define BUF_SIZE 1024
//
// int main(int argc, char** argv)
// {
//     if (argc != 3)
//         usageErr("%s file file\n", argv[0]);
//
//     int fd1 = open(argv[1], O_RDONLY);
//     if (fd1 == -1)
//         errExit("open");
//
//     int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0600);
//     if (fd2 == -1)
//         errExit("open");
//
//
//     char buf[BUF_SIZE];
//     while (true)
//     {
//         // FIGURE OUT DATA / HOLE SIZES
//         int numRead;
//
//         int currentOffset = lseek(fd1, 0, SEEK_CUR);
//         if (currentOffset == -1)
//             errExit("lseek");
//
//         int dataOffset = lseek(fd1, currentOffset, SEEK_DATA);
//         if (dataOffset == -1)
//             break; // NO MORE DATA
//
//         if (currentOffset == dataOffset)
//         {
//             int holeOffset = lseek(fd1, currentOffset, SEEK_HOLE);
//             if (holeOffset == -1)
//                 errExit("lseek");
//
//             if (lseek(fd1, currentOffset, SEEK_SET) == -1)
//                 errExit("lseek");
//
//             numRead = holeOffset - currentOffset;
//         }
//         else
//         {
//             if (lseek(fd2, dataOffset, SEEK_SET) == -1)
//                 errExit("lseek");
//
//             continue;
//         }
//
//         // WRITE DATA
//         int rv;
//
//         while (numRead > BUF_SIZE)
//         {
//             rv = read(fd1, buf, BUF_SIZE);
//             if (rv == -1)
//                 errExit("read");
//
//             numRead -= BUF_SIZE;
//
//             rv = write(fd2, buf, rv);
//             if (rv == -1)
//                 errExit("write");
//         }
//
//         rv = read(fd1, buf, numRead);
//         if (rv == -1)
//             errExit("read");
//
//         rv = write(fd2, buf, rv);
//         if (rv == -1)
//             errExit("write");
//     }
//
//     if (close(fd1) == -1) errExit("close");
//     if (close(fd2) == -1) errExit("close");
//
//     exit(EXIT_SUCCESS);
// }


