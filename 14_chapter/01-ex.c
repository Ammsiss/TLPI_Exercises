/*

    PROGRAM: //////////////////////////////////////////////////////////////////////

    Write a program that measures the time required to create and then remove a
    large number of 1-byte files from a single directory.

    The program should create files with names of the form xNNNNNN, where NNNNNN is
    replaced by a random six-digit number.

    The files should be created in the random order in which their names are
    generated, and then deleted in increasing numerical order (i.e., an order that is
    different from that in which they were created).

    The number of files (NF) and the directory in which they are to be created should be
    specifiable on the command line.

    TESTS /////////////////////////////////////////////////////////////////////////

    Measure the times required for different values of NF (e.g., in the range from
    1000 to 20,000) and for different file systems (e.g., ext2, ext3, and XFS).

    What patterns do you observe on each file system as NF increases?

    How do the various file systems compare?

    Do the results change if the files are created in increasing numerical order
    (x000001, x000001, x0000002, and so on) and then deleted in the same order?
    If so, what do you think the reason(s) might be? Again, do the results vary
    across file-system types?
*/

/*

   PERFORMANCE METRICS:

FS ext2 (Random order of deletion):
    1'000  Files: 0.00user 0.77system 0:00.78elapsed 99%CPU
    5'000  Files:
    10'000 Files:
    20'000 Files:
FS ext2 (Consistent order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:

Patterns:

FS ext3 (Random order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:
FS ext3 (Consistent order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:

Patterns:

FS ext4 (Random order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:
FS ext4 (Consistent order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:

Patterns:

FS xfs (Random order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:
FS xfs (Consistent order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:

Patterns:

FS btrfs (Random order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:
FS btrfs (Consistent order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:

Patterns:

FS tmpfs (Random order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:
FS tmpfs (Consistent order of deletion):
    1'000  Files:
    5'000  Files:
    10'000 Files:
    20'000 Files:

Patterns:
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/dynamic_array.h"

struct FileInfo
{
    int number;
    int fd;
    char path[4096];
};

int file_type_exists(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
        errExit("stat");

    // return (sb.st_mode & S_IFMT) == S_IFREG;
    return S_ISREG(sb.st_mode);
}

int directory_exists(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
        errExit("stat");

    // return (sb.st_mode & S_IFMT) == S_IFDIR;
    return S_ISDIR(sb.st_mode);
}

char digitToChar(int num)
{
    if (num < 0 || num > 9)
        return '\0';

    return (char)(num + 48);
}

void numToStr(int num, char *buf)
{
    size_t index = 0;
    char reverseBuf[4096];
    while (num > 0) {
        int digit = num % 10;
        num /= 10;

        reverseBuf[index] = digitToChar(digit);

        ++index;
    }

    for (size_t i = index - 1, y = 0; (int)i >= 0; --i, ++y)
        buf[i] = reverseBuf[y];

    buf[index] = '\0';
}

int fileSort(const void *a, const void *b)
{
    const struct FileInfo *f1 = a;
    const struct FileInfo *f2 = b;

    return f1->number - f2->number;
}

int main(int argc, char** argv) {
    if (argc != 3)
        usageErr("01-ex.bin NF <dirpath>");

    char *endptr;
    int numFiles = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || numFiles > 899999)
        usageErr("01-ex.bin NF <dirpath>");

    if (!directory_exists(argv[2]))
        usageErr("Please enter a valid directory");

    char dirPath[4096];
    strcpy(dirPath, argv[2]);

    if (argv[2][strlen(argv[2]) - 1] != '/')
        strcat(dirPath, "/");

    DynArray fileList;
    da_init(&fileList, 0, sizeof(struct FileInfo));

    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec ^ tv.tv_usec);

    for (int i = 0; i < numFiles; ++i) {

        char filePrefix[2] = "x";
        char fileNumber[10];

        int randomNumber = (rand() % 900000) + 100000; // [100'000, 999'999]
        numToStr(randomNumber, fileNumber);

        char filePath[4096];
        strcpy(filePath, dirPath);
        strcat(filePath, filePrefix);
        strcat(filePath, fileNumber);

        int fd;
        if ((fd = open(filePath, O_CREAT | O_EXCL | O_RDWR, 0600)) == -1) {
            if (errno == EEXIST) {
                --i;
                continue;
            } else
                errExit("open");
        }
        write(fd, "X", 1);

        struct FileInfo *fileInfo = da_push(&fileList);
        strcpy(fileInfo->path, filePath);
        fileInfo->fd = fd;
        fileInfo->number = randomNumber;
    }

    da_sort(&fileList, &fileSort);

    for (size_t i = 0; i < fileList.size; ++i) {
        struct FileInfo *fileInfo = da_get(&fileList, i);
        unlink(fileInfo->path);
        close(fileInfo->fd);
    }

    printf("Created and destroyed %zu files.\n", fileList.size);

    struct tms processTime;
    double clockTicks = sysconf(_SC_CLK_TCK);

    times(&processTime);
    double systemTime = (double)processTime.tms_stime / clockTicks;
    double userTime = (double)processTime.tms_utime / clockTicks;

    printf("user:%.2f system:%.2f cpu_total:%.2f\n", userTime, systemTime,
        userTime + systemTime);

    da_free(&fileList);
}
