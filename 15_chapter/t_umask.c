#include <stdio.h>
#define _GNU_SOURCE

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/limits.h>
#include <fcntl.h>
#include <unistd.h>

#include "error_functions.h"
#include "file_perms.h"

#define MYFILE "myfile"
#define MYDIR  "mydir"

#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) // 0660
#define DIR_PERMS (S_IRWXU | S_IRWXG | S_IRWXO) // 0777
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH) // 0033

int main(void)
{
    int fd;
    struct stat sb;
    mode_t u;

    umask(UMASK_SETTING);

    fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
    if (fd == -1)
        errExit("open-%s", MYFILE);
    if (mkdir(MYDIR, DIR_PERMS) == -1)
        errExit("mkdir-%s", MYDIR);

    u = umask(0); /* Retrieves (and clears) umask value */

    if (stat(MYFILE, &sb) == -1)
        errExit("stat-%s", MYFILE);

    printf("Requested file perms: %s\n", filePermStr(FILE_PERMS, 0));
    printf("Process umask:        %s\n", filePermStr(u, 0));
    printf("Actual file perms:    %s\n\n", filePermStr(sb.st_mode, 0));

    if (stat(MYDIR, &sb) == -1)
        errExit("stat-%s", MYDIR);
    printf("Requested dir. perms: %s\n", filePermStr(DIR_PERMS, 0));
    printf("Process umask:        %s\n", filePermStr(u, 0));
    printf("Actual dir. perms:    %s\n\n", filePermStr(sb.st_mode, 0));

    unlink(MYFILE);
    rmdir(MYDIR);

    exit(EXIT_SUCCESS);
}
