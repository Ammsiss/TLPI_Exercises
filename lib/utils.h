#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <malloc.h>

#include "dynamic_array.h"
#include "error_functions.h"

inline static int getNum(const char *numStr, char *msg)
{
    char *end;

    errno = 0;
    int num = strtol(numStr, &end, 10);
    if (errno == ERANGE)
        errExit("strtol");

    if (*end != '\0')
        errExit("%s", (msg == NULL) ? "strtol" : msg);

    return num;
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

inline static int file_exists(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
        errExit("stat");

    // return (sb.st_mode & S_IFMT) == S_IFREG;
    return S_ISREG(sb.st_mode);
}

inline static int directory_exists(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
        errExit("stat");

    // return (sb.st_mode & S_IFMT) == S_IFDIR;
    return S_ISDIR(sb.st_mode);
}

inline static char digitToChar(int num)
{
    if (num < 0 || num > 9)
        return '\0';

    return (char)(num + 48);
}

inline static void numToStr(int num, char *buf)
{
    size_t index = 0;
    char reverseBuf[4096];
    while (num > 0) {
        int digit = num % 10;
        num /= 10;

        reverseBuf[index] = digitToChar(digit);

        ++index;
    }

    for (size_t i = index - 1, y = 0; (int)i >= 0; --i, ++y)
        buf[i] = reverseBuf[y];

    buf[index] = '\0';
}

char* str_insert(char *buf, const char *base, const char *str,
    size_t index)
{
    if (!buf || !base || !str)
        return NULL;

    size_t i;
    for (i = 0; i < index; ++i) {
        buf[i] = base[i];
    }
    buf[i] = '\0';

    strcat(buf, str);

    for (i = index; base[i] != '\0'; ++i) {
        buf[i + strlen(str)] = base[i];
    }
    buf[i + strlen(str)] = '\0';

    return buf;
}

#endif
