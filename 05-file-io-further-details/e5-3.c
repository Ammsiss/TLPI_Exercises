////////////////////////////////////////////////////////////////////////
// 5.14 - 5-3. Why atomicity with O_APPEND is necessary.
////////////////////////////////////////////////////////////////////////

// int main(int argc, char** argv)
// {
//     if (argc != 3 && argc != 4)
//         usageErr("atomic_append filename num-bytes [x]1\n");
//
//     bool noAppend = false;
//     if (argc == 4)
//     {
//         if (strcmp(argv[3], "x") != 0)
//             usageErr("atomic_append filename num-bytes [x]2\n");
//
//         noAppend = true;
//     }
//
//     int flags = O_WRONLY | O_CREAT;
//     if (!noAppend)
//         flags |= O_APPEND;
//
//     int fd = open(argv[1], flags, 0600);
//     if (fd == -1)
//         errExit("open");
//
//     char* errPointer;
//
//     int numBytes = strtol(argv[2], &errPointer, 10);
//     if (*errPointer != '\0')
//         usageErr("atomic_append filename num-bytes [x]3\n");
//
//     for (int x = 0; x < numBytes; ++x)
//     {
//         if (noAppend)
//             if (lseek(fd, 0, SEEK_END) == -1)
//                 errExit("lseek");
//
//         if (write(fd, "X", 1) == -1)
//             errExit("write");
//     }
//
//     if (close(fd) == -1)
//         errExit("close");
//
//     return EXIT_SUCCESS;
// }
