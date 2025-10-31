#define GNU_SOURCE

#include <string.h>
#include <malloc.h>

#include <pthread.h>
#include <stdio.h>

#include "../lib/error_functions.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t dirname_key;
static pthread_key_t basename_key;

static void dest(void *buf)
{
    free(buf);
}

void createKeyDirname(void)
{
    int s;
    if ((s = pthread_key_create(&dirname_key, dest)) != 0)
        errExitEN(s, "pthread_key_create");
}

void createKeyBasename(void)
{
    int s;
    if ((s = pthread_key_create(&basename_key, dest)) != 0)
        errExitEN(s, "pthread_key_create");
}

char *dirname_ts(char *path)
{
    int s;
    if ((s = pthread_once(&once, createKeyDirname)) != 0)
        errExitEN(s, "pthread_once");

    char *buf = pthread_getspecific(dirname_key);
    if (buf == NULL) {
        buf = malloc(256);
        if (buf == NULL)
            errExit("malloc");

        if ((s = pthread_setspecific(dirname_key, buf)) != 0)
            errExitEN(s, "pthread_setspecific");
    }

    int path_len = strlen(path);

    if (path_len == 0)
        return "";
    if (path_len > 255)
        return NULL;

    int base_len = 0;
    for (int i = path_len - 1; i >= 0; --i) {
        ++base_len;
        if (path[i] == '/')
            break;
    }

    int i = 0;
    while (i < path_len - base_len) {
        buf[i] = path[i];
        ++i;
    }
    buf[i] = '\0';

    return buf;
}

char *basename_ts(char *path)
{
    int s;
    if ((s = pthread_once(&once, createKeyBasename)) != 0)
        errExitEN(s, "pthread_once");

    char *buf = pthread_getspecific(basename_key);
    if (buf == NULL) {
        buf = malloc(256);
        if (buf == NULL)
            errExit("malloc");

        if ((s = pthread_setspecific(basename_key, buf)) != 0)
            errExitEN(s, "pthread_setspecific");
    }

    int path_len = strlen(path);

    if (path_len == 0)
        return "";
    if (path_len > 255)
        return NULL;

    int y = 0;
    for (int i = path_len - 1; i >= 0; --i) {
        if (path[i] == '/')
            break;

        buf[y] = path[i];
        ++y;
    }
    buf[y] = '\0';

    for (int start = 0, end = strlen(buf) - 1; start < end; ++start, --end) {
        int swap = buf[start];
        buf[start] = buf[end];
        buf[end] = swap;
    }

    return buf;
}

int main(void)
{
    char mod_arr[256] = "/1/2/3/4/HELLO";

    char *basename = basename_ts(mod_arr);
    printf("%s\n", basename);

    char *dirname = dirname_ts(mod_arr);
    printf("%s\n", dirname);
}
