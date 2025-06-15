#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static inline void errExit(const char *fmt, ...) __attribute__((format(printf, 1, 2), noreturn));

static inline void errExit(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, ": %s\n", strerror(errno));
    va_end(args);

    exit(EXIT_FAILURE);
}
