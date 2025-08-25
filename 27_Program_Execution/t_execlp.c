#include <unistd.h>

#include "../lib/error_functions.h"

int main(int argc, char **argv)
{
    if (argc != 2)
        usageErr("%s command", argv[0]);

    execlp(argv[1], argv[1], (char *) NULL);
    errExit("execlp");
}
