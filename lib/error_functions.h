#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* This attribute style is only
 * supported if __GNUC__ is defined */

#define NORETURN __attribute__ ((__noreturn__))
#define PFFORMAT(x, y) __attribute__ ((format(printf, (x), (y))))

void errMsg(const char *fmt, ...) PFFORMAT(1, 2);
void errExit(const char *fmt, ...) PFFORMAT(1, 2);
void err_exit(const char *format, ...) PFFORMAT(1, 2);
void errExitEN(int errnum, const char *fmt, ...) PFFORMAT(2, 3);
void fatal(const char *format, ...) PFFORMAT(1, 2);
void usageErr(const char *fmt, ...) PFFORMAT(1, 2);
void cmdLineErr(const char *fmt, ...) PFFORMAT(1, 2);

#endif
