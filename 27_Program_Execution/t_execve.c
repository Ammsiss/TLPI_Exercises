#include <linux/limits.h>
#include <string.h>
#include <unistd.h>

#include "../lib/dynamic_array.h"
#include "../lib/error_functions.h"

#define ARG_LEN 255

extern char **environ;

int main(int argc, char **argv)
{
    DynArray argv_list;   /* Don't have to free if execve suceeds */
    da_init(&argv_list, 0, sizeof(char *));

    for (int i = 1; i < argc; ++i) {
        char **element = (char **) da_push(&argv_list);
        *element = malloc(strlen(argv[i]) + 1);
        strcpy(*element, argv[i]);
    }

    char **lastElement = (char **) da_push(&argv_list);
    *lastElement = NULL;

    char **p_argv = (char **) da_get(&argv_list, 0);
    execve(argv[1], p_argv, environ);

    /* Must have failed */

    da_free(&argv_list);
    errExit("execve");
}
