#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static inline void errExit(const char *fmt, ...)
    __attribute__((format(printf, 1, 2), noreturn));

static inline void errExit(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, ": %s\n", strerror(errno));

    exit(EXIT_FAILURE);
}

static inline void usageErr(const char *fmt, ...)
    __attribute__((format(printf, 1, 2), noreturn));

static inline void usageErr(const char *fmt, ...)
{
    fprintf(stderr, "Usage: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");

    exit(EXIT_FAILURE);
}

static inline void messExit(const char *fmt, ...)
    __attribute__((format(printf, 1, 2), noreturn));

static inline void messExit(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");

    exit(EXIT_FAILURE);
}
