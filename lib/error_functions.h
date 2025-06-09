#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errExit(const char* msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}
