/*
Name:
    imp_chattr - modify file inode flags

Usage:
    chattr [mode] files...

Description:
    This implementation was created as a solution to TLPI exersise 15-7.

    The format of a symbolic mode is +-=[aAcCdDeFijmPsStTux].

    The  operator  '+' causes the selected attributes to be added to the existing
    attributes of the files; '-' causes them to be removed; and '=' causes them
    to be the only  attributes that the files have.

    The  letters  'aAcCdDeFijmPsStTux'  select the new attributes for the files:
    append only (a), no atime updates (A), compressed (c), no copy on write (C),
    no dump  (d),  synchronous  directory  updates (D), extent format (e),
    case-insensitive directory lookups (F), immutable (i), data journaling (j),
    don't compress (m), project  hierarchy  (P),  secure deletion  (s),
    synchronous updates (S), no tail-merging (t), top of directory hierarchy
    (T), undeletable (u), and direct access for files (x).

    For more information such as a detailed description of each attribute see
    man CHATTR(1).
*/

#include <unistd.h>
#define _GNU_SOURCE

#include <linux/fs.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "error_functions.h"

int main(int argc, char **argv)
{
    if (argc < 3)
        usageErr("./imp_chattr [mode] files...");

    int flags = 0;
    int modeLength = strlen(argv[1]);

    if (modeLength == 1)
        usageErr("No flags specified");

    for (int i = 1; i < modeLength; ++i) {
        switch (argv[1][i]) {
        case 'a': flags |= FS_APPEND_FL;       break;
        case 'c': flags |= FS_COMPR_FL;        break;
        case 'D': flags |= FS_DIRSYNC_FL;      break;
        case 'i': flags |= FS_IMMUTABLE_FL;    break;
        case 'j': flags |= FS_JOURNAL_DATA_FL; break;
        case 'A': flags |= FS_NOATIME_FL;      break;
        case 'd': flags |= FS_NODUMP_FL;       break;
        case 't': flags |= FS_NOTAIL_FL;       break;
        case 's': flags |= FS_SECRM_FL;        break;
        case 'S': flags |= FS_SYNC_FL;         break;
        case 'T': flags |= FS_TOPDIR_FL;       break;
        case 'u': flags |= FS_UNRM_FL;         break;
        default:
            usageErr("./imp_chattr [mode] files...");
        }
    }

    int errFnd = 0;

    for (int i = 2; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "open->%s: ", argv[i]);
                perror("");
                errFnd = 1;
                continue;
            }

        int currentFlags;

        if (ioctl(fd, FS_IOC_GETFLAGS, &currentFlags) == -1) {
            perror("ioctl");
            errFnd = 1;
        }

        switch(argv[1][0]) {
        case '+': currentFlags |= flags; break;
        case '-': currentFlags &= ~flags; break;
        case '=': currentFlags = flags; break;
        default:
            usageErr("./imp_chattr [mode] files...");
        };

        if (ioctl(fd, FS_IOC_SETFLAGS, &currentFlags) == -1) {
            perror("ioctl");
            errFnd = 1;
        }

        close(fd);
    }

    return errFnd ? EXIT_FAILURE : EXIT_SUCCESS;
}
