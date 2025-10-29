# Chapter 6: Exercises
## Ex. 6-1

**Question**

Compile the program in Listing 6-1 (mem_segments.c), and list its size using ls –l.
Although the program contains an array (mbuf) that is around 10 MB in size, the
executable file is much smaller than this. Why is this?

**Answer**

See *initialized_mem.c*

## Ex. 6-2

**Question**

Write a program to see what happens if we try to longjmp() into a function that has
already returned.

**Answer**

See *bad_longjmp.c*

## Ex. 6-3

**Question**

Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary,
code that directly modifies environ. Your version of unsetenv() should check to see
whether there are multiple definitions of an environment variable, and remove
them all (which is what the glibc version of unsetenv() does).

**Answer**

See *setenv_unsetenv_imp.c*
