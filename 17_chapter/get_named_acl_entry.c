/*

*/

#include <getopt.h>
#include <sys/types.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    gid_t gid = 0;
    uid_t uid = 0;

    int uid_flag = 0;
    int gid_flag = 0;

    int opt;
    char *endptr;
    while ((opt = getopt(argc, argv, "u:g:")) != -1) {
        switch (opt) {
        case 'u':
            uid = strtol(optarg, &endptr, 10);
            if (*endptr != '\0')
                errExit("strol");
            uid_flag = 1;
            break;
        case 'g':
            gid = strtol(optarg, &endptr, 10);
            if (*endptr != '\0')
                errExit("strol");
            gid_flag = 1;
            break;
        case '?': usageErr("get_named_acl_entry.bin [ug] name file"); break;
        }
    }

    if ((gid_flag && uid_flag) || optind + 1 != argc)
        usageErr("get_named_acl_entry.bin [ug] name file");

    exit(EXIT_SUCCESS);
}
