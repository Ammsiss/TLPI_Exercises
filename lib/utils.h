#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <malloc.h>

#include "dynamic_array.h"

inline static int fgetln(int fd, char **buf)
{
    DynArray chars;
    da_init(&chars, 0, sizeof(char));

    int numRead = 0;

    while (1) {
        char c;
        int rv = read(fd, &c, 1);
        if (rv == -1) {
            da_free(&chars);
            return -1;
        }

        char *element = (char *)da_push(&chars);
        if (element == NULL) {
            da_free(&chars);
            return -1;
        }

        ++numRead;

        if (rv == 0 || c == '\n') {
            *element = '\0';
            break;
        }

        *element = c;
    }

    *buf = (char *)da_detach(&chars);
    return numRead;
}

inline static char* fgetstr(int fd)
{
    DynArray chars;
    da_init(&chars, 0, sizeof(char));

    while (1) {
        char *c = (char *)da_push(&chars);
        if (c == NULL) {
            da_free(&chars);
            return NULL;
        }

        int rv = read(fd, c, 1);
        if (rv == -1) {
            da_free(&chars);
            return NULL;
        }

        if (rv == 0 || *c == '\n') {
            *c = '\0';
            break;
        }
    }

    return (char *)da_detach(&chars);
}

#endif
