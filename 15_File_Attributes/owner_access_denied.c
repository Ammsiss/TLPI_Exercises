#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error_functions.h"

#define MYFILE "myfile"

int main(void)
{
    umask(0);   /* Clear umask */

    int fd = open(MYFILE, O_CREAT | O_RDWR, 0066);   /* Create with no owner permisssions */
    if (fd == -1)
        errExit("open");

    close(fd);

    /* Call open again to test */

    if (open(MYFILE, O_RDWR)) {
        perror("Write failed");
    }

    unlink(MYFILE);

    exit(EXIT_SUCCESS);
}
