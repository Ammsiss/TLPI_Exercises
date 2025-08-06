/*
Name:
    imp_getcwd

SYNOPSIS:
    char *imp_getcwd(char buf[.size], size_t size);

Description:
    This implementation was created as a solution to TLPI exersise 18-5.

    The following program assigns the current working directory in a buffer
    'size' bytes long that the user passes in.

    On success the function returns a pointer to the buffer. If size is less
    then the length of the path including the terminating null byte NULL
    is returned and errno is set to ERANGE. If the size of the path including
    the terminating null byte is greater than PATH_MAX then NULL is returned
    and errno is set to ENAMETOOLONG.
*/

#define GNU_SOURCE_
#define _POSIX_C_SOURCE 200809L

#include <limits.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "../lib/error_functions.h"
#include "../lib/dynamic_array.h"

char *imp_getcwd(char *cwdbuf, size_t size)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX) == NULL)
        return NULL;

    DynArray pathComponents;
    da_init(&pathComponents, 0, NAME_MAX + 1);

    size_t pathLength = 1;   /* Check size against pathname including terminating null byte */

    while (1) {

        struct stat current_sb;
        if (stat(".", &current_sb) == -1)
            return NULL;

        if (chdir("..") == -1)
            return NULL;

        struct stat parent_sb;
        if (stat(".", &parent_sb) == -1)
            return NULL;

        if (current_sb.st_ino == parent_sb.st_ino && current_sb.st_dev == parent_sb.st_dev)
            break;

        DIR *dir = opendir(".");
        if (dir == NULL)
            return NULL;

        struct dirent *entry;
        while (1) {
            errno = 0;
            if ((entry = readdir(dir)) == NULL)   /* break on error or no more directories */
                break;

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            /* Remember that the inode stored in dirent always refers to the
               directory before any moints are applied. Therefore you should
               not compare the inode stored in dirent with the one stored in
               the stat object as that one does reflect potential mounts. */

            struct stat entry_sb;
            if (stat(entry->d_name, &entry_sb) == -1)
                return NULL;

            if (entry_sb.st_ino == current_sb.st_ino) {

             /* We must also check the device number in the case that the current
                directory is a mount point. If it is then the inode number alone
                is not sufficient as inodes are only unique on a single file system. */

                if (entry_sb.st_dev == current_sb.st_dev) {
                    char *component = da_push(&pathComponents);
                    strcpy(component, entry->d_name);
                    pathLength += strlen(entry->d_name) + 1;   /* + 1 for '/' */

                    break;
                }
            }
        }

        if (closedir(dir) == -1)
            errExit("closedir");

        if (errno != 0)
            errExit("readdir");
    }

    if (size < pathLength) {
        errno = ERANGE;
        return NULL;
    }

    if (pathLength >= PATH_MAX) {
        errno = ENAMETOOLONG;
        return NULL;
    }

    strcpy(cwdbuf, "/");
    for (int i = pathComponents.size - 1; i >= 0; --i) {
        char *component = da_get(&pathComponents, i);
        strcat(cwdbuf, component);

        if (i != 0)
            strcat(cwdbuf, "/");
    }

    da_free(&pathComponents);

    if (chdir(cwd) == -1)
        errExit("chdir");

    return cwdbuf;
}

int main(void)
{
    char *cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd);

    char path[PATH_MAX + 1];   /* + 1 for null byte */
    if (imp_getcwd(path, PATH_MAX + 1) == NULL)
        errExit("imp_getcwd");

    // printf("%s\n", path);

    cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd);
}
