/*
Name:
    effective_access.bin - Determine access permissions of a file

Usage:
    effective_access.bin -erwx [FILE]

Description:
    Uses my implementation of the access(2) function to determine the
    permissions of a file passed in as a command line argument. The implementation
    determiens access based on the effective user and group id's rather then the real
    access(2) implementation which does so with the real effective and group id's

Options:
    -e, Check if file exists
    -r, Check if has read permission
    -w, Check if has write permission
    -x, Check if has execute permission

Return:
    0 on success, -1 on error or at least one of the requested permission checks failed.

Example:
    ./effective_access.bin -rx myfile.txt
        -> Checks if the executing process can read and execute 'myfile.txt'.
*/

#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "error_functions.h"

#define EXIST 1
#define HAS_R 2
#define HAS_W 4
#define HAS_X 8

#define R_PERM_GRP 0
#define W_PERM_GRP 1
#define X_PERM_GRP 2

struct Group {
    int user;
    int group;
    int other;
};

int checkPermGroup(const struct Group permGroup, const struct stat *sb)
{
    uid_t uid = geteuid();
    gid_t gid = getegid();

    if (uid == sb->st_uid) {
        if (!(sb->st_mode & permGroup.user))
            return -1;
    } else if (gid == sb->st_gid) {
        if (!(sb->st_mode & permGroup.group))
            return -1;
    } else if (!(sb->st_mode & permGroup.other))
        return -1;

    return 0;
}

int effective_access(const char* filename, const uint8_t flags)
{
    if (flags == 0) // no flags set
        return -1;

    struct stat sb;
    if (stat(filename, &sb) == -1) {
        if (errno == EACCES)   /* No execute perms on one of the dir's in the path prefix */
            return -1;
        else if (errno == ENOENT) {   /* A portion of the pathname doesn't exist */
            return -1;
        }
    }

    struct Group permGroups[3] = {
        { .user = S_IRUSR, .group = S_IRGRP, .other = S_IROTH }, // Read
        { .user = S_IWUSR, .group = S_IWGRP, .other = S_IWOTH }, // Write
        { .user = S_IXUSR, .group = S_IXGRP, .other = S_IXOTH }  // Exectue
    };

    int rv = 0;
    if (flags & HAS_R)
         rv += checkPermGroup(permGroups[R_PERM_GRP], &sb);
    if (flags & HAS_W)
        rv += checkPermGroup(permGroups[W_PERM_GRP], &sb);
    if (flags & HAS_X)
        rv += checkPermGroup(permGroups[X_PERM_GRP], &sb);

    return (rv != 0) ? -1 : 0;
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
        usageErr("effective_access.bin [-erwx] <FILE>");

    int fileIndex = 1;
    uint8_t flags = 0;

    if (argc == 3) {
        fileIndex = 2;

        int optionLen = strlen(argv[1]);
        for (int i = 1; i < optionLen; ++i) {
            switch (argv[1][i]) {
            case 'e': flags |= EXIST; break;
            case 'r': flags |= HAS_R; break;
            case 'w': flags |= HAS_W; break;
            case 'x': flags |= HAS_X; break;
            default:
                usageErr("effective_access.bin [erwx] <FILE>");
            }
        }
    } else
        flags |= HAS_R | HAS_W | HAS_X;

    if (effective_access(argv[fileIndex], flags) == -1) {
        printf("One or more permission checks failed!\n");
        return EXIT_FAILURE;
    }
    else {
        printf("Success!\n");
        return EXIT_SUCCESS;
    }
}
