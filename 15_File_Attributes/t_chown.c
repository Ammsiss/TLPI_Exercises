#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#include "ugid_functions.h"
#include "error_functions.h"

typedef int Boolean;

int main(int argc, char **argv)
{
    uid_t uid;
    gid_t gid;
    int j;
    Boolean errFnd;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s owner group [file...]\n"
                "        owner or group can be '-', "
                "meaning leave unchanged\n", argv[0]);

    if (strcmp(argv[1], "-") == 0) {   /* "-" -> don't change owner */
        uid = -1;
    } else {                           /* Turn user name into UID */
        uid = userIdFromName(argv[1]);
        if (uid == -1U)
            errExit("No such user (%s)", argv[1]);
    }

    if (strcmp(argv[2], "-") == 0) {   /* "-" -> don't change group */
        gid = -1;
    } else {                           /* Turn group name ionto GID */
        gid = groupIdFromName(argv[2]);
        if (gid == -1U)
            errExit("No group user (%s)", argv[1]);
    }

    /* Change ownership of all files named in remaining arguments */

    errFnd = 0;
    for (j = 3; j < argc; j++) {
        if (chown(argv[j], uid, gid) == -1) {
            errExit("chown: %s", argv[j]);
            errFnd = 1;
        }
    }

    exit(errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}
