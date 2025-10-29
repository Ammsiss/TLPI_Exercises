#define _GNU_SOURCE

#include "stdio.h"

#include <utime.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "error_functions.h"

#define MYFILE "myfile"

int main(void)
{
    int fd = open(MYFILE, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1)
        errExit("open");

    write(fd, "X", 1);
    lseek(fd, 0, SEEK_SET);

    printf("Sleeping for 2 seconds...\n\n");
    sleep(2);

    char buf[1];
    read(fd, buf, 1); /* Update access time stamp (must mount with strictatime) */

    struct stat sb;
    stat(MYFILE, &sb);

    printf("The timestamps of the file before utime() call:\n\n");
    printf("Last access time:         %s"
           "Last Modification time:   %s"
           "Last status change time:  %s\n",
        ctime(&sb.st_atime), ctime(&sb.st_mtime), ctime(&sb.st_ctime));

    struct utimbuf ub;
    ub.actime = sb.st_atim.tv_sec; /* Leave access time unchanged */
    ub.modtime = sb.st_atim.tv_sec;

    utime(MYFILE, &ub); /* status change time is always set to current time */

    stat(MYFILE, &sb);

    printf("The timestamps of the file after utime() call:\n\n");
    printf("Last access time:        %s"
           "Last Modification time:  %s"
           "Last status change time: %s",
        ctime(&sb.st_atime), ctime(&sb.st_mtime), ctime(&sb.st_ctime));

    if (unlink(MYFILE) == -1)
        errExit("unlink");

    exit(EXIT_SUCCESS);
}
