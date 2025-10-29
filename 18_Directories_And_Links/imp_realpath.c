/*
Name:
    imp_realpath

SYNOPSIS:
    char *imp_realpath(const char *pathname, char *resolved_path)

Description:
    This implementation was created as a solution to TLPI exersise 18-3.

    This program resolves any relative directories (., ..) and symlinks,
    assinging the resulting absolute path into the 'resolved_path' buffer
    which must be at least PATH_MAX + 1 bytes long.

    On success the function returns a pointer to the buffer.

    On error the function returns NULL and sets errno to the appropriate
    value.
*/

#define _XOPEN_SOURCE 500

#include <errno.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>

#include "../lib/error_functions.h"

char *chDir(const char *pathname, char *resolved_path)
{
    char base_cpy[PATH_MAX + 1];
    strcpy(base_cpy, pathname);
    char* basepart = basename(base_cpy);

    char dir_cpy[PATH_MAX + 1];
    strcpy(dir_cpy, pathname);
    char* dirpart = dirname(dir_cpy);

    if (chdir(dirpart) == -1)
        return NULL;

    char realpath[PATH_MAX + 1];
    if (getcwd(realpath, PATH_MAX + 1) == NULL)
        return NULL;

    strcpy(resolved_path, realpath);
    strcat(resolved_path, "/");
    strcat(resolved_path, basepart);

    return resolved_path;
}

// Not handling readlink truncation //

char *imp_realpath(const char *pathname, char *resolved_path)
{
    char cwd[PATH_MAX + 1];
    if (getcwd(cwd, PATH_MAX + 1) == NULL)
        return NULL;

    if (chdir(pathname) == -1) {
        if (errno == ENOTDIR) {
            struct stat sb;
            if (lstat(pathname, &sb) == -1)
                return NULL;

            switch (sb.st_mode & S_IFMT) {
            case S_IFLNK:
            {
                char link_content[PATH_MAX + 1];
                int numRead = readlink(pathname, link_content, PATH_MAX + 1);
                if (numRead == -1)
                    return NULL;
                link_content[numRead] = '\0';

                char path_cpy[PATH_MAX + 1];
                strcpy(path_cpy, pathname);
                char *dirpart = dirname(path_cpy);

                if (chdir(dirpart) == -1)
                    return NULL;

                imp_realpath(link_content, resolved_path);

                break;
            }
            default:
            {
                chDir(pathname, resolved_path);
                break;
            }
            }
        } else {
            return NULL;
        }
    } else {
        char realpath[PATH_MAX + 1];
        if (getcwd(realpath, PATH_MAX + 1) == NULL)
            return NULL;

        strcpy(resolved_path, realpath);
    }

    if (chdir(cwd) == -1)
        return NULL;

    return resolved_path;
}

int main(void)
{
    char realpath[PATH_MAX + 1];
    if (imp_realpath("/home//////", realpath) == NULL)
        errExit("imp_realpath");

    printf("%s\n", realpath);
}
