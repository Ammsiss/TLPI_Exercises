#include "error_functions.h"
#include "ename.c.inc"   /* Defines ename array and MAX_ENAME */
#include "tlpi_hdr.h" // IWYU pragma: export

NORETURN
static void terminate(int use_exit3)
{
    char *s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (use_exit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void outputError(int use_err, int err, int flush_stdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE];
    char user_msg[BUF_SIZE];
    char err_text[BUF_SIZE];

    vsnprintf(user_msg, BUF_SIZE, format, ap);

    if (use_err)
        snprintf(err_text, BUF_SIZE, " [%s %s]",
                (err > 0 && err < MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(err_text, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", err_text, user_msg);

    if (flush_stdout)
        fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
}

/* Print an error messaage (with errno message) */

void errMsg(const char *fmt, ...)
{
    va_list ap;
    int saved_errno = errno;

    va_start(ap, fmt);
    outputError(1, errno, 1, fmt, ap);
    va_end(ap);

    errno = saved_errno;
}

/* Print an error message and terminate (with errno message) */

void errExit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    outputError(1, errno, 1, fmt, ap);
    va_end(ap);

    terminate(1);
}

/* Display error message including 'errno' diagnostic, and
   terminate the process by calling _exit().

   The relationship between this function and errExit() is analogous
   to that between _exit(2) and exit(3): unlike errExit(), this
   function does not flush stdout and calls _exit(2) to terminate the
   process (rather than exit(3), which would cause exit handlers to be
   invoked).

   These differences make this function especially useful in a library
   function that creates a child process that must then terminate
   because of an error: the child must terminate without flushing
   stdio buffers that were partially filled by the caller and without
   invoking exit handlers that were established by the caller. */

void err_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    outputError(1, errno, 0, fmt, ap);
    va_end(ap);

    terminate(0);
}

/* The following function does the same as errExit(), but expects
   the error number in 'errnum' this is especially useful for the
   functions in the pthread api */

void errExitEN(int errnum, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    outputError(1, errnum, 1, fmt, ap);
    va_end(ap);

    terminate(1);
}

/* Print an error message and terminate (no errno message) */

void fatal(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    outputError(0, 0, 1, fmt, ap);
    va_end(ap);

    terminate(1);
}

/* Print a command usage error message and terminate the process */

void usageErr(const char *fmt, ...)
{
    va_list ap;

    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

/* Diagnose an error in command-line arguments and
   terminate the process */

void cmdLineErr(const char *fmt, ...)
{
    va_list ap;

    fflush(stdout);

    fprintf(stderr, "Command-line usage error: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fflush(stderr);           /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}
