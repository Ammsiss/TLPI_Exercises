/*
*/

#define _DEFAULT_SOURCE   /* for S_IFMT */

#include "error_functions.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("./arX_mod.bin [FILE]");

    struct stat sb;
    if (stat(argv[1], &sb) == -1)
        errExit("stat");

    mode_t mode = sb.st_mode;

    bool hasOneExe = (mode & S_IXUSR) | (mode & S_IXGRP) | (mode & S_IXOTH);

    if (S_ISDIR(sb.st_mode) || hasOneExe)
        mode |= S_IXUSR | S_IXGRP | S_IXOTH;

    mode |= S_IRUSR | S_IRGRP | S_IROTH;

    mode &= 07777; // Clear irrelevent filetype bits
    chmod(argv[1], mode);
}
