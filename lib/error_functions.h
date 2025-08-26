#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

inline static void errExit(const char *fmt, ...)
    __attribute__((format(printf, 1, 2), noreturn));

inline static void errExit(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, ": %s\n", strerror(errno));

    exit(EXIT_FAILURE);
}

inline static void errExitEN(int errnum, const char *fmt, ...)
    __attribute__((format(printf, 2, 3), noreturn));

inline static void errExitEN(int errnum, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, ": %s\n", strerror(errnum));

    exit(EXIT_FAILURE);
}

inline static void usageErr(const char *fmt, ...)
    __attribute__((format(printf, 1, 2), noreturn));

inline static void usageErr(const char *fmt, ...)
{
    fprintf(stderr, "Usage: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");

    exit(EXIT_FAILURE);
}

inline static void messExit(const char *fmt, ...)
    __attribute__((format(printf, 1, 2), noreturn));

inline static void messExit(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");

    exit(EXIT_FAILURE);
}

#endif
