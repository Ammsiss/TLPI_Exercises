#include <unistd.h>

#include "../lib/error_functions.h"

extern char **environ;

int main(void)
{
                    /* argv[0] is ignored */
    char *p_argv[] = { "run_interp", "god", "hello", "world" };
    execve("./echo_args.script", p_argv, environ);
    errExit("execve");
}
