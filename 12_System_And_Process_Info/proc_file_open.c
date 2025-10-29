/*
    Write a program that lists all processes that have a particular file pathname open.

    This can be achieved by inspecting the contents of all of the /proc/PID/fd/(*) symbolic
    links.

    This will require nested loops employing readdir(3) to scan all /proc/PID
    directories, and then the contents of all /proc/PID/fd entries within each /proc/PID
    directory.

    To read the contents of a /proc/PID/fd/n symbolic link requires the use
    of readlink(), described in Section 18.5.
*/

#define _GNU_SOURCE

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "error_functions.h"
#include "utils.h"

void getName(char *buf, const char *path)
{
    int fd;
    if ((fd = open(path, O_RDONLY)) == -1)
        errExit("open");

    char *name = fgetstr(fd);
    if (!name)
        errExit("fgetstr");
    strcpy(buf, name);

    free(name);
}

bool isNumber(char *str)
{
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}

int main(int argc, char** argv)
{
    if (argc != 2)
        usageErr("03-ex.bin <path>");

    DIR *procStream = opendir("/proc");
    struct dirent *dirEntry;

    DynArray processList;
    da_init(&processList, 0, 256);

    errno = 0;
    while ((dirEntry = readdir(procStream))) {
        if (!isNumber(dirEntry->d_name))
            continue;

        // /proc/<PID>
        char procPath[256] = "/proc/";
        strcat(procPath, dirEntry->d_name);

        // /proc/<PID>/comm
        char commPath[256] = "";
        strcat(commPath, procPath);
        strcat(commPath, "/comm");

        // /proc/<PID>/fd
        char fdPath[256] = "";
        strcat(fdPath, procPath);
        strcat(fdPath, "/fd/");

        DIR *fdStream = opendir(fdPath);
        if (fdStream == NULL)
            errExit("opendir");

        bool hasOpenFile = false;

        errno = 0;
        while ((dirEntry = readdir(fdStream))) {
            if (!isNumber(dirEntry->d_name))
                continue;

            char linkPath[256] = "";
            strcat(linkPath, fdPath);
            strcat(linkPath, dirEntry->d_name);

            char linkContent[4096];
            ssize_t numRead = readlink(linkPath, linkContent, 4096);
            if (numRead == -1)
                errExit("readlink");
            linkContent[numRead] = '\0';

            hasOpenFile = strcmp(linkContent, argv[1]) == 0;

            if (hasOpenFile) {
                char *buf = da_push(&processList);
                if (!buf)
                    errExit("da_push");

                getName(buf, commPath);
                break;
            }
        }
        closedir(fdStream);
        if (errno != 0)
            errExit("readdir");
    }
    closedir(procStream);
    if (errno != 0)
        errExit("readdir");

    for (size_t i = 0; i < processList.size; ++i) {
        char *name = da_get(&processList, i);
        printf("%s\n", name);
    }
}
