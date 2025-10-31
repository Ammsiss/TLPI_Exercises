#include <string.h>
#define GNU_SOURCE

#include <pthread.h>
#include <stdio.h>

static __thread char dirn_buf[256];
static __thread char basn_buf[256];

char *dirname_ts(char *path)
{
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

    printf("pathlen: %d\n", path_len);
    printf("baselen: %d\n", base_len);

    int i = 0;
    while (i < path_len - base_len) {
        dirn_buf[i] = path[i];
        ++i;
    }
    dirn_buf[i] = '\0';

    return dirn_buf;
}

char *basename_ts(char *path)
{
    int path_len = strlen(path);

    if (path_len == 0)
        return "";
    if (path_len > 255)
        return NULL;

    int y = 0;
    for (int i = path_len - 1; i >= 0; --i) {
        if (path[i] == '/')
            break;

        basn_buf[y] = path[i];
        ++y;
    }
    basn_buf[y] = '\0';

    for (int start = 0, end = strlen(basn_buf) - 1; start < end; ++start, --end) {
        int swap = basn_buf[start];
        basn_buf[start] = basn_buf[end];
        basn_buf[end] = swap;
    }

    return basn_buf;
}

int main(void)
{
    char mod_arr[256] = "yeet//hello";
    char *basename = dirname_ts(mod_arr);

    printf("%s\n", basename);
}
