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

#define DEFAULT_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) // 0660
#define DESIRED_MODE (S_IRUSR | S_IWUSR)


int main(void)
{
    struct stat st;

    umask(0); // clear umask

    int fd = open(MYFILE, O_CREAT | O_EXCL | O_RDWR, DEFAULT_MODE);
    if (fd == -1)
        errExit("open-%s", MYFILE);

    stat(MYFILE, &st);
    printf("File mode prior to chmod call: %s\n", filePermStr(st.st_mode, 0));

    /* In order to change permissions, the process must be privileged or its effective uid
       must match the owner (uid) of the file. */
    chmod(MYFILE, DESIRED_MODE);

    stat(MYFILE, &st);
    printf("File mode after first chmod call: %s\n", filePermStr(st.st_mode, 0));

    mode_t modifiedPerm = st.st_mode | S_IXOTH | S_IWOTH; // turn on indvidual bits
    modifiedPerm = modifiedPerm & ~S_IWOTH; // turn of individual bit
    chmod(MYFILE, modifiedPerm);

    stat(MYFILE, &st);
    printf("File mode after second chmod call: %s\n", filePermStr(st.st_mode, 0));

    unlink(MYFILE);
    exit(EXIT_SUCCESS);
}
