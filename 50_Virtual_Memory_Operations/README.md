# Chapter 50: Virtual Memory Operations
## Ex. 50-1

**Question**

Verify the effect of the RLIMIT_MEMLOCK resource limit by
writing a program that sets a value for this limit and then
attempts to lock more memory than the limit

**Answer**

See *over_lock_limit.c*

## Ex. 50-2

**Question**

Write a program to verify the operation of the madvise()
MADV_DONTNEED operation for a writable MAP_PRIVATE mapping.

**Answer**

See *dont_need_mem.c*

On linux it simply zeroed out the memory.
