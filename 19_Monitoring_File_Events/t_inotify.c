#include <linux/limits.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../lib/error_functions.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("t_inotify directory");

    struct stat sb;
    if (stat(argv[1], &sb) == -1)
        errExit("stat");

    if (!S_ISDIR(sb.st_mode))
        usageErr("t_inotify directory");

    int inotify = inotify_init();
    if (inotify == -1)
        errExit("inotify_init");

    int wd = inotify_add_watch(inotify, argv[1], IN_DELETE | IN_CREATE);
    if (wd == -1)
        errExit("inotify_add_watch");

    struct inotify_event *event;
    char buf[BUF_LEN];

    while (1) {
        int numRead = read(inotify, buf, BUF_LEN);
        if (numRead == 0)
            errExit("read() from inotify fd returned 0!");

        if (numRead == -1)
            errExit("read");

        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        for (char *p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;

            printf("Event: ");
            if (event->mask & IN_CREATE) printf("IN_CREATE\n");
            if (event->mask & IN_DELETE) printf("IN_DELETE\n");

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    close(inotify);

    exit(EXIT_SUCCESS);
}
