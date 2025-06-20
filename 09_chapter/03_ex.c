////////////////////////////////////////////////////////////////////////
// 9.9 - 9-3. Reimplementing initgroups() with setgroups()
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <limits.h>
#include <unistd.h>
#include <grp.h>

#include "../lib/error_functions.h"

int initgroups_clone(const char *user, gid_t group)
{
    gid_t grouplist[NGROUPS_MAX];
    grouplist[0] = group;

    setgrent();
    int i = 1;
    struct group *grent;
    while ((grent = getgrent()) != NULL) {
        if (grent->gr_mem != NULL) {
            for (char **member = grent->gr_mem; *member !=  NULL; ++member) {
                if (strcmp(user, *member) == 0) {
                    grouplist[i] = grent->gr_gid;
                    ++i;
                    break;
                }
            }

            if (i == NGROUPS_MAX)
                return -1;
        }
    }
    endgrent();

    return setgroups(1, grouplist);
}

void printSupGroups()
{
    setgrent();
    struct group *grent;
    while ((grent = getgrent()) != NULL) {
        printf("%s, %d:\n", grent->gr_name, grent->gr_gid);

        if (grent->gr_mem != NULL)
            for (char **member = grent->gr_mem; *member != NULL; ++member)
                printf("    %s\n", *member);
    }
    endgrent();
}

int main(void)
{
    printf("Prior to calling initgroups():\n");
    printSupGroups();

    long lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)
        lnmax = 256;

    char *username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");

    printf("Enter username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE); // exit on EOF

    size_t len = strlen(username);
    if (username[len - 1] == '\n')
        username[len - 1] = '\0';

    if (initgroups_clone(username, 1000) == -1)
        errExit("initgroups_clone");

    printf("\n\nAfter calling initgroups():\n");
    printSupGroups();
}
