/*
Name:
    tree_watch.c

Usage:
    tree_watch directory

Description:
    This implementation was created as a solution to TLPI exersise 19-1.

    The following program logs various information about a directory and
    all of its subdirectories.
*/

/*
    Handle deletions, insertions and moves of sub directories
*/

#define _XOPEN_SOURCE 600       /* Get nftw() and S_IFSOCK declarations */

#include <linux/limits.h>
#include <sys/inotify.h>
#include <ftw.h>
#include <unistd.h>
#include <fcntl.h>

#include "../lib/error_functions.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static int inotifyFd;

static int addWatch(const char *pathname, const struct stat *sbuf, int type,
    struct FTW *ftwb)
{
    if (type != FTW_D)
        return 0;

    int wd = inotify_add_watch(inotifyFd, pathname,
        IN_ISDIR | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE_SELF);
    if (wd == -1)
        errExit("inotify_add_watch");

    printf("Watching %s using wd %d\n", pathname, wd);

    return 0;
}

static void refreshWatches(const char *path)
{
    if (nftw(path, &addWatch, 20, FTW_PHYS) == -1)
        errExit("nftw");
}

static void displayEvent(struct inotify_event *event)
{
    if (event->mask & IN_ISDIR)
        printf("d ");

    if (event->len > 0)
        printf("%s\n", event->name);
}

static int handleEvent(struct inotify_event *event, const char *path)
{
    if (event->mask & IN_ISDIR) {
        if (event->mask & IN_CREATE && event->len > 0) {
            close(inotifyFd);
            inotifyFd = inotify_init();
            refreshWatches(path);
            return 1;
        } else if (event->mask & IN_DELETE_SELF) {
            inotify_rm_watch(inotifyFd, event->wd);
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("tree_watch directory");

    inotifyFd = inotify_init();
    if (inotifyFd == -1)
        errExit("inotify_init");

    refreshWatches(argv[1]);

    char buf[BUF_LEN];
    while (1) {
        ssize_t numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0) {
            fprintf(stderr, "read()->0 from inotify fd!");
            exit(EXIT_FAILURE);
        }

        if (numRead == -1)
            errExit("read");

        struct inotify_event *event;
        for (char *p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;

            displayEvent(event);
            if (handleEvent(event, argv[1]) == 1)
                break;;

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    close(inotifyFd);
}
