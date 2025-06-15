////////////////////////////////////////////////////////////////////////
// 7.4 - 7-2. Reimplementing malloc() and free().
////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/uio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../lib/error_functions.h"

static int32_t blocks_allocated = 0;

// void free(void *ptr)
// {
//
// }

/*
Limitations:
    - Breaks if sbrk() < (4 + what the user asked fo)
*/

void *malloc_clone(size_t size)
{
    void *block = sbrk(size);
    uintptr_t start_of_block = (uintptr_t)block;

    if ((int)start_of_block == -1)
        errExit("sbrk");

    ++blocks_allocated;

    int32_t *meta_data = block;
    *meta_data = blocks_allocated;
    meta_data = (int*)((char*)block + 4);
    *meta_data = (int32_t)((uintptr_t)sbrk(0) - start_of_block);

    void *user_start = (char*)block + 8;
    return user_start;
}

int main(void)
{
    uintptr_t initial_break = (uintptr_t)sbrk(0);
    printf("Current heapsize: %lu\n", (uintptr_t)sbrk(0) - initial_break);

    // First allocation
    char* heap_mem = malloc_clone(20);
    for (int i = 0; i < 10; ++i) {
        heap_mem[i] = 'X';
        printf("%c ", heap_mem[i]);
    }
    puts("");
    printf("Block #%d\n", *(int*)(heap_mem - 8));
    printf("Block size:%d\n", *(int*)(heap_mem - 4));
    printf("Current heapsize: %lu\n\n", (uintptr_t)sbrk(0) - initial_break);

    // Second allocation
    char* heap_mem2 = malloc_clone(20);
    for (int i = 0; i < 10; ++i) {
        heap_mem2[i] = 'X';
        printf("%c ", heap_mem2[i]);
    }
    puts("");
    printf("Block #%d\n", *(int*)(heap_mem2 - 8));
    printf("Block size:%d\n", *(int*)(heap_mem2 - 4));
    printf("Current heapsize: %lu\n\n", (uintptr_t)sbrk(0) - initial_break);

    // Second allocation
    char* heap_mem3 = malloc_clone(20);
    for (int i = 0; i < 10; ++i) {
        heap_mem3[i] = 'X';
        printf("%c ", heap_mem3[i]);
    }
    puts("");
    printf("Block #%d\n", *(int*)(heap_mem3 - 8));
    printf("Block size:%d\n", *(int*)(heap_mem3 - 4));
    printf("Current heapsize: %lu\n\n", (uintptr_t)sbrk(0) - initial_break);
}
