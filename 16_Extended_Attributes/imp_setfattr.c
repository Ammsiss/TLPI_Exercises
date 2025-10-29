/*
Name:
    imp_setfattr.bin - Create or set an existing extended attribute of a file.

Usage:
    imp_setfattr.bin -n name -v value [FILE]

Return:
    0 on success, -1 on error.

Example:
    ./imp_setfattr.bin -n user.attribute -v "1234" myfile.txt
        -> Sets the attribute user.attribute="1234" or updates it if it exists.
*/

#include <getopt.h>
#include <stdlib.h>
#include <sys/xattr.h>

#include "error_functions.h"

#define XATTR_SIZE 10000

int main(int argc, char **argv)
{
    opterr = 0;   /* Silence default error messages */
    int opt;

    char *nameArg = NULL;
    char *valueArg = NULL;

    while ((opt = getopt(argc, argv, "n:v:")) != -1) {
        switch (opt) {
        case 'n':
            nameArg = optarg;
            break;
        case 'v':
            valueArg = optarg;
            break;
        case '?':
            usageErr("imp_setfattr -n <name> [-v <value>] pathname...");
        }
    }

    if (!nameArg)
        usageErr("imp_setfattr -n <name> [-v <value>] pathname...");

    if (!valueArg)
        valueArg = "";

    setxattr(argv[optind], nameArg, valueArg, strlen(valueArg), 0);

    exit(EXIT_SUCCESS);
}
