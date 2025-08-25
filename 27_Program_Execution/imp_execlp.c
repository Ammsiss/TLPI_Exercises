#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/dynamic_array.h"
#include "../lib/error_functions.h"

extern char **environ;

int imp_execlp(const char *file, const char *arg, ...)
{
    DynArray argv_list;
    da_init(&argv_list, 0, sizeof(char *));

    char procName[PATH_MAX];
    strcpy(procName, arg);

    char **first = da_push(&argv_list);
    *first = procName;

    va_list va;
    va_start(va, arg);

    while (1) {
        char **element = da_push(&argv_list);
        if (!element) {
            da_free(&argv_list);
            return -1;
        }

        char *argument = va_arg(va, char *);
        *element = argument;

        if (!argument)
            break;
    }

    /* As a security measure, the superuser account (root) is
        normally set up so that the current working directory
        is excluded from PATH. */

    char *path_env = getenv("PATH");

    char defaultPath[] = ".:/usr/bin:/bin";
    if (!path_env)   /* If PATH is not set use default value */
        path_env = defaultPath;

    /* SUSv3 stipulates that an application should not modify
        the string returned by getenv(). This is because this
        string is actually part of the environment */

    char *path = malloc(strlen(path_env) + 1);
    if (!path) {
        da_free(&argv_list);
        va_end(va);
        return -1;
    }

    strcpy(path, path_env);

    char *token = strtok(path, ":");
    while (token != NULL) {

        char *fullPath = malloc(strlen(token) + strlen(file) + 2);
        if (!fullPath) {
            da_free(&argv_list);
            va_end(va);
            return -1;
        }

        strcpy(fullPath, token);
        strcat(fullPath, "/");
        strcat(fullPath, file);

        execve(fullPath, first, environ);

        /* File doesn't exist or is not executable, try next path item */

        free(fullPath);
        token = strtok(NULL, ":");
    }

   /* File could not be found, cleanup return -1 */

    free(path);
    da_free(&argv_list);
    va_end(va);

    return -1;
}

int main(void)
{
    imp_execlp("ls", "ls", "-l", (char *) NULL);
    errExit("execlp");
}
