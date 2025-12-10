#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

static char ttyname_buf[NAME_MAX + 1];

static char *scan_dir(const char *scan_dir, struct stat *sb)
{
    DIR *dir = opendir(scan_dir);
    if (dir == NULL)
        return NULL;

    int found = 0;
    struct stat sb_ent;
    struct dirent *dp;

    while (1) {
        errno = 0;
        if ((dp = readdir(dir)) == NULL && errno == 0)
            break;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        /* Fine to compare inode stored in dirent with that of the stat object
         * because we are only comapring against files so no mount point shen-
         * anaigans as observed in ../18_Directories_And_Links/imp_getcwd.c */

        if (dp->d_ino == sb->st_ino) {
            strcpy(ttyname_buf, scan_dir);
            strcat(ttyname_buf, "/");
            strcat(ttyname_buf, dp->d_name);
            if (stat(ttyname_buf, &sb_ent) == -1)
                errExit("stat");
            if (sb_ent.st_dev == sb->st_dev) {
                found = 1;
                break;
            }
        }
    }

    if (closedir(dir) == -1)
        return NULL;

    return (found) ? ttyname_buf : NULL;
}

char *ttyname_imp(int fd)
{
    struct stat stat_buf;
    if (fstat(fd, &stat_buf) == -1)
        errExit("fstat");

    if (scan_dir("/dev", &stat_buf) != NULL)
        return ttyname_buf;
    if (scan_dir("/dev/pts", &stat_buf) != NULL)
        return ttyname_buf;

    return NULL;
}

int main(void)
{
    char *ttyname;
    if ((ttyname = ttyname_imp(STDOUT_FILENO)) == NULL)
        errExit("ttyname_imp");

    printf("%s\n", ttyname);
}
