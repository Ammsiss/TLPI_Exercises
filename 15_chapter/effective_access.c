/*
    A program that tests my implementation of the access(2) function. (which checks against
    effective user and group id's instead of real)

Usage: effective_access.bin -a [FILE]

Options:
    -a -> Displays all permission checks

Example:
*/

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "error_functions.h"

// Need to check user and group and all permissions to check need to be specificable
// does access() short circuit? Also probably should check supplementary groups
int effective_access(const char* filename)
{
    bool badPermFnd = 0;

    struct stat sb;
    if (stat(filename, &sb) == -1) {
        if (errno == EACCES)   /* No execute perms on one of the dir's in the path prefix */
            badPermFnd = 1;
        else if (errno == ENOENT) {   /* A portion of the pathname prefix doesn't exist */
            badPermFnd = 1;
        }
    }

    /* Check if executing process has read permissions on the file */

    if (sb.st_mode & S_IRUSR) { // If file allows user to read
        uid_t uid = geteuid(); // Check if executing process eu-id matches file u-id

        if (sb.st_uid != uid) { // -> false validated
            if (sb.st_mode & S_IRGRP) { // if file allows group to read
                gid_t gid = getegid();

                if (sb.st_gid != gid) { // Check if exe process eg-id matches file gid
                    if (!(sb.st_mode & S_IROTH))
                        badPermFnd = 1;
                }
            }
        }
    } else

    return badPermFnd ? -1 : 1;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("effective_access.bin -a [FILE]");

    effective_access(argv[1]);

    return EXIT_SUCCESS;
}
