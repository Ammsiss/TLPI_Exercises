////////////////////////////////////////////////////////////////////////
// 6.10 - 6-1.
/*

On executino of ls -l the size of the binary is very small (~10kb) even
though we allocate an array of 10MB. This is becuase the array is stored
in bss where it is not actually phsically allocated. This allows the binary
to remain lean.

If you initialize the array then the binary WILL grow as it will be stored in
the data segment of memory.

*/
////////////////////////////////////////////////////////////////////////

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];            /* Uninitialized data segment */
int primes[] = { 2, 3, 5, 7 };  /* Initialized data segment */

static int
square(int x)                   /* Allocated in frame for square() */
{
    int result;                 /* Allocated in frame for square() */

    result = x * x;
    return result;              /* Return value passed via register */
}

static void
doCalc(int val)                 /* Allocated in frame for doCalc() */
{
    printf("The square of %d is %d\n", val, square(val));

    if (val < 1000) {
        int t;                  /* Allocated in frame for doCalc() */

        t = val * val * val;
        printf("The cube of %d is %d\n", val, t);
    }
}

int
main(int argc, char *argv[])    /* Allocated in frame for main() */
{
    static int key = 9973;      /* Initialized data segment */
    static char mbuf[10240000]; /* Uninitialized data segment */
    char *p;                    /* Allocated in frame for main() */

    p = malloc(1024);           /* Points to memory in heap segment */

    doCalc(key);

    exit(EXIT_SUCCESS);
}
