#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/error_functions.h"

int callCount = 0;

void func1(void)
{
    ++callCount;
    printf("Func1: Exit handler call count: %d\n", callCount);
}

void func2(void)
{
    ++callCount;
    printf("Func2: Call count: %d\n", callCount);
}

int main(void)
{
    printf("Registering func1 3 times.\n");
    for (int i = 0; i < 3; ++i)
        if (atexit(func1) != 0)
            errExit("atexit");

    printf("Registering func2 3 times.\n");
    for (int i = 0; i < 3; ++i)
        if (atexit(func2) != 0)
            errExit("atexit");

#ifndef CALL_EXIT
    exit(EXIT_SUCCESS);
#else
    _exit(EXIT_SUCCESS);
#endif
}
