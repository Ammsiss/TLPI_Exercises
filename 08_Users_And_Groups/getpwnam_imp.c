#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "error_functions.h"

static struct passwd *getpwnam_clone(const char *name)
{
    setpwent(); // Reset back to start of passwd file

    if (name == NULL)
        return NULL;

    static struct passwd *out = { 0 };

    struct passwd *entry;
    while (1) {
        entry = getpwent(); // Get next passwd entry

        if (entry == NULL) {
            out = NULL;
            break;
        }

        if (strcmp(entry->pw_name, name) == 0) {
            out = entry;
            break;
        }
    }

    endpwent(); // close stream
    return out;
}

int main(void)
{
    long lnmax = sysconf(_SC_LOGIN_NAME_MAX);

    if (lnmax == -1)
        lnmax = 256;

    char *username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");

    printf("Username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE);

    size_t len = strlen(username);
    if (username[len - 1] == '\n')
        username[len - 1] = '\0';

    errno = 0;
    struct passwd *passwd = getpwnam_clone(username);
    if (passwd == NULL) {
        if (errno == 0)
            printf("No Entry!\n");
        else
            errExit("getpwnam");
    }
    else
        printf("%s, %s, %u\n", passwd->pw_name, passwd->pw_shell, passwd->pw_uid);
}
