#if ! defined(__sun)
#define _DEFAULT_SOURCE /* Get getpass() declaration from <unistd.h> */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE   /* Get crypt() declaration from <unistd.h> */
#endif
#endif

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <limits.h>
#include <sys/types.h>
#include <grp.h>

#include "../lib/error_functions.h"

/* Fun Fact: this program is trivially root compromsing! Not something you
 * would want to have globally executable thats for sure. */

int main(int argc, char **argv)
{
    if (argc < 2)
        usageErr("%s [-u user] program-file arg1 arg2 ...", argv[0]);

    char *user = NULL;

    int program_index = 1;

    if (strcmp(argv[1], "-u") == 0) {

        if (argc < 4)
            usageErr("%s [-u user] program-file arg1 arg2 ...", argv[0]);

        program_index = 3;
        user = argv[2];
    }

    if (user != NULL) {   /* If null then just run as root */
        struct passwd *pwd = getpwnam(user);
        if (pwd == NULL)
            errExit("getpwuid");

        struct spwd *spwd = getspnam(user);   /* Need to have root privileges to call */
        if (spwd == NULL && errno == EACCES)
            errExit("getspnam");

        if (spwd != NULL)   /* If shadow passwd is in use, then use it */
            pwd->pw_passwd = spwd->sp_pwdp;

        /* Note* Should use readpassphrase(3bsd) instead */
        char *password = getpass("Enter Password: ");
        char *encrypted = crypt(password, pwd->pw_passwd);

        /* Clear plain text password */
        for (char *p = password; *p != '\0'; )
            *p++ = '\0';

        if (encrypted == NULL)
            errExit("crypt");

        int authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
        if (!authOk) {
            printf("Incorrect password\n");
            exit(EXIT_FAILURE);
        }

        printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

        /* This is a privileged process so it will also set the real and saved uid */
        if (setuid(pwd->pw_uid) == -1)
            errExit("setuid");

        /* Replicate all group ownership */
        initgroups(pwd->pw_name, pwd->pw_gid);

        /* At this point the process is efectively executing as if it was run by 'user' */
    }

    execvp(argv[program_index], &argv[program_index]);
    errExit("execvp");
}
