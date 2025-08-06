/*
    - Dereference all symbolic links in 'pathname' (a null terminated string)
      and resolve all references to /. and /..

    - Resolved path is a buffer of at least PATH_MAX bytes

    - On success return pointer to resolved_path

    - (Optional) Add glibc function feature of passing in NULL for buf to make
      function allocate for you.


*/

#include <errno.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../lib/dynamic_array.h"
#include "../lib/error_functions.h"

void charcat(char *str, char c)
{
    char cstr[2];
    cstr[0] = c;
    cstr[1] = '\0';

    strcat(str, cstr);
}

int isdir(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
        return -1;

    return S_ISDIR(sb.st_mode);
}

/* Doesn't work for links */
char *imp_realpath(const char *pathname, char *resolved_path)
{
    char *cwd = getcwd(NULL, 0);

    if (pathname[0] == '/')
        if (chdir("/") == -1)
            return NULL;

    char link_value[PATH_MAX];
    char comp[NAME_MAX + 1] = "";
    for(const char *c = pathname; *c != '\0'; ++c) {
        if (*c == '/') {
            if (strcmp(comp, "") != 0) {
                if (isdir(comp)) {
                    if (chdir(comp) == -1)
                        return NULL;
                    strcpy(comp, "");
                }
            }
        } else {
            charcat(comp, *c);
        }
    }

    char *filepath = NULL;
    if (strcmp(comp, "") != 0) {
        if (isdir(comp)) {
            if (chdir(comp) == -1)
                return NULL;
        } else {
            filepath = comp;
        }
    }

    char *realpath = getcwd(NULL, 0);
    strcpy(resolved_path, realpath);

    if (filepath) {
        strcat(resolved_path, "/");
        strcat(resolved_path, filepath);
    }

    if (chdir(cwd) == -1)
        errExit("chdir");

    free(cwd);
    free(realpath);

    return resolved_path;
}

int main(void)
{
    char realpath[PATH_MAX + 1];
    if (imp_realpath("./link", realpath) == NULL)
        errExit("imp_realpath");

    printf("%s\n", realpath);
}
