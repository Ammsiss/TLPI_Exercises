## Syscalls

`brk(2)` - Set the program break  
`sbrk(2)` - increment the program break

`malloc(3)` - dyn-alloc memory from the heap  
`free(3)` - free memory previously allocated by malloc()

`calloc(3)` - `malloc()` but cooler  
`realloc(3)` - frees and reallocates an existing memory block

`memalign(3)` - allocate memory aligned to a specific power  
`posix_memalign(3)`

`alloca(3)` - stack allocate memory

## Tables

**Figure 7-1**: Memory block returned by `malloc()`  
**Figure 7-1**: Memory block returned by `malloc()`  
**Figure 7-3**: Heap containing allocated blocks and a free list

## More Info

## TODO

- [ ] Test *dynamic_array.c*
