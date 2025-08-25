#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char **argv)
{
    printf("---Start of New program---\n");
    printf("Program name: %s\n", argv[0]);

    if (argc == 1)
        printf("No args supplied.");
    else
        for (int i = 1; i < argc; ++i)
            printf("Arg %d: %s\n", i, argv[i]);

    printf("\nEnv List:\n");
    for (char **ep = environ; *ep != NULL; ++ep)
        printf("%s\n", *ep);

    exit(EXIT_SUCCESS);
}
