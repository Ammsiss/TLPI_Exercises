#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#include <stdarg.h>

/* This attribute style is only
 * supported if __GNUC__ is defined */

#define NORETURN __attribute__ ((__noreturn__))
#define PFFORMAT(x, y) __attribute__ ((format(printf, (x), (y))))

/* Print an error messaage (with errno message) */
void errMsg(const char *fmt, ...) PFFORMAT(1, 2);
/* Print an error message and terminate (with errno message) */
void errExit(const char *fmt, ...) PFFORMAT(1, 2);
/* Display error message including 'errno' diagnostic, and
   terminate the process by calling _exit(). */
void err_exit(const char *format, ...) PFFORMAT(1, 2);
/* The following function does the same as errExit(), but expects
   the error number in 'errnum' */
void errExitEN(int errnum, const char *fmt, ...) PFFORMAT(2, 3);
/* Print an error message and terminate (no errno message) */
void fatal(const char *format, ...) PFFORMAT(1, 2);
/* Print a command usage error message and terminate the process */
void usageErr(const char *fmt, ...) PFFORMAT(1, 2);
/* Diagnose an error in command-line arguments and
   terminate the process */
void cmdLineErr(const char *fmt, ...) PFFORMAT(1, 2);

#endif
