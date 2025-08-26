#include <pthread.h>

#include "../lib/error_functions.h"

int main(void)
{
    int s;
    if ((s = pthread_join(pthread_self(), NULL)) != 0)
        errExitEN(s, "pthread_join");

    exit(EXIT_SUCCESS);
}
