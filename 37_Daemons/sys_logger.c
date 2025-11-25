/*
    LOG_EMERG
    LOG_ALERT
    LOG_CRIT
    LOG_ERR
    LOG_WARNING
    LOG_NOTICE
    LOG_INFO
    LOG_DEBUG
*/

#include "../lib/error_functions.h"
#include <syslog.h>

#define LEVEL_COUNT 8

int main(int argc, char **argv)
{
    if (argc < 2)
        usageErr("%s [level] MESSAGE", argv[0]);

    char *level_strings[LEVEL_COUNT] = { "LOG_EMERG", "LOG_ALERT", "LOG_CRIT",
        "LOG_ERR", "LOG_WARNING", "LOG_NOTICE", "LOG_INFO", "LOG_DEBUG" };
    int level_vals[LEVEL_COUNT] = { LOG_EMERG, LOG_ALERT, LOG_CRIT,
        LOG_ERR, LOG_NOTICE, LOG_INFO, LOG_DEBUG };

    int log_level = LOG_NOTICE;

    if (argc == 3) {
        for (int i = 0; i < LEVEL_COUNT; ++i) {
            if (strcmp(argv[1], level_strings[i]) == 0) {
                log_level = level_vals[i];
                break;
            }
        }
    }

    openlog(argv[0], 0, LOG_USER);   /* LOG_USER is the default */
    syslog(log_level | LOG_USER, "%s", argv[2]);
    closelog();
}
