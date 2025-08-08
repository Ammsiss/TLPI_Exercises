/*
Name:
    tree_info

Usage:
    tree_info directory

Description:
    This implementation was created as a solution to TLPI exersise 18-7.

    The following program uses nftw to traverse a directory tree collecting
    information about the counts of various file types and finishes by printing
    the counts and percentage.
*/

#define _XOPEN_SOURCE 500

#include <ftw.h>

#include "../lib/error_functions.h"

int regCount = 0;
int dirCount = 0;
int chrCount = 0;
int blkCount = 0;
int lnkCount = 0;
int fifoCount = 0;
int sockCount = 0;

int processFunc(const char *pathname, const struct stat *statbuf, int typeflag,
    struct FTW *ftwbuf)
{
    switch (statbuf->st_mode & S_IFMT) {
    case S_IFREG: ++regCount;   break;
    case S_IFDIR: ++dirCount;   break;
    case S_IFCHR: ++chrCount;   break;
    case S_IFBLK: ++blkCount;   break;
    case S_IFLNK: ++lnkCount;   break;
    case S_IFIFO: ++fifoCount;  break;
    case S_IFSOCK: ++sockCount; break;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("tree_info [DIR]");

    if (nftw(argv[1], &processFunc, 20, FTW_PHYS) == -1)
        errExit("nftw");

    int total = 0;
    total += regCount;
    total += dirCount;
    total += chrCount;
    total += blkCount;
    total += lnkCount;
    total += fifoCount;
    total += sockCount;

    if (total == 0) {
        printf("No files found.\n");
        exit(EXIT_SUCCESS);
    }

    printf("Total Files:          %d->%.1f%%\n", regCount, (float) regCount / total * 100);
    printf("Total Directories:    %d->%.1f%%\n", dirCount, (float) dirCount / total * 100);
    printf("Total Chr devices :   %d->%.1f%%\n", chrCount, (float) chrCount / total * 100);
    printf("Total Blk devices:    %d->%.1f%%\n", blkCount, (float) blkCount / total * 100);
    printf("Total Symlinks:       %d->%.1f%%\n", lnkCount, (float) lnkCount / total * 100);
    printf("Total FIFOs or pipes: %d->%.1f%%\n", fifoCount, (float) fifoCount / total * 100);
    printf("Total Sockets:        %d->%.1f%%\n", sockCount, (float) sockCount / total * 100);

    exit(EXIT_SUCCESS);
}
