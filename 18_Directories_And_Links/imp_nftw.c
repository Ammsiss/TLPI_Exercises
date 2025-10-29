/*
Name:
    imp_nftw

SYNOPSIS:
    int nftw(const char *dirpath,
            int (*fn)(const char *fpath, const struct stat *sb,
                        int typeflag, struct FTW *ftwbuf),
            int nopenfd, int flags);

Description:
    This implementation was created as a solution to TLPI exersise 18-8.
*/

/*
    This will require the use of the opendir(), readdir(), closedir(), and
    stat() system calls, among others.

TESTS
    Empty directories
    Symlink loops
    Permission denied errors
    Files of all types
    Deep directory trees
*/

#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "../lib/error_functions.h"

struct FTW
{
    int base;    /* Offset to basename part of pathname */
    int level;   /* Depth of file within tree traversal */
};

int imp_nftw(const char *dirpath, int (*fn)(const char *fpath, const struct stat *sb
    /*, int typeflag, struct FTW *ftwbuf*/)/*, int nopenfd, int flags*/)
{
    DIR *dir = opendir(dirpath);
    if (dir == NULL)
        return -1;

    struct dirent *entry;
    while (1) {
        errno = 0;
        if ((entry = readdir(dir)) == NULL)
            break;

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char entry_path[PATH_MAX + 1];
        strcpy(entry_path, dirpath);
        if (strcmp(dirpath, "/") != 0)
            strcat(entry_path, "/");
        strcat(entry_path, entry->d_name);

        struct stat sb;
        if (lstat(entry_path, &sb) == -1)
            break;

        if (entry->d_type == DT_DIR) {   /* If directory, recurse */
            imp_nftw(entry_path, fn);
            if (fn(entry_path, &sb) == 1)
                break;
        } else {   /* If file call user defined func */
            if (fn(entry_path, &sb) == 1)
                break;
        }
    }
    closedir(dir);

    if (errno != 0)
        return -1;

    return 1;
}

int testFunc(const char *fpath, const struct stat *sb)
{
    switch (sb->st_mode & S_IFMT) {
    case S_IFREG:  printf("-"); break;
    case S_IFDIR:  printf("d"); break;
    case S_IFCHR:  printf("c"); break;
    case S_IFBLK:  printf("b"); break;
    case S_IFLNK:  printf("l"); break;
    case S_IFIFO:  printf("p"); break;
    case S_IFSOCK: printf("s"); break;
    default:       printf("?"); break; /* Should never happen (on Linux) */
    }

    printf(" %s\n", fpath);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("imp_nftw directory");

    if (imp_nftw(argv[1], &testFunc) == -1)
        errExit("imp_nftw");
}
