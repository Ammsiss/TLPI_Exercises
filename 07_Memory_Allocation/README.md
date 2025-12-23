# Chapter 7: Exercises

## Ex. 7-1

**Question**

Modify the program in **Listing 7-1** (*free_and_sbrk.c*) to
print out the current value of the program break after each
execution of `malloc()`. Run the program specifying a small
allocation block size. This will demonstrate that `malloc()`
doesn’t employ `sbrk()` to adjust the program break on each
call, but instead periodically allocates larger chunks of memory
from which it passes back small pieces to the caller.

**Answer**

The following is the output I got after printing the current
program break after each malloc call with a block size of 10,000
and step size of 1 using **Listing 7-1** (*free_and_sbrk.c*).

At call #14 `malloc()` increased the program break by 139,264
bytes even though we only asked for 10,000. This is to avoid
frequent calls to `sbrk()`, which would have a greater
performance impact.

```
Initial program break:           0x8891000
Allocating 25*10000 bytes

malloc() #1:  143200256
malloc() #2:  143200256
malloc() #3:  143200256
malloc() #4:  143200256
malloc() #5:  143200256
malloc() #6:  143200256
malloc() #7:  143200256
malloc() #8:  143200256
malloc() #9:  143200256
malloc() #10: 143200256
malloc() #11: 143200256
malloc() #12: 143200256
malloc() #13: 143200256
malloc() #14: 143339520 <- allocation
malloc() #15: 143339520
malloc() #16: 143339520
malloc() #17: 143339520
malloc() #18: 143339520
malloc() #19: 143339520
malloc() #20: 143339520
malloc() #21: 143339520
malloc() #22: 143339520
malloc() #23: 143339520
malloc() #24: 143339520
malloc() #25: 143339520

Program break is now:            0x88b3000
Freeing blocks from 1 to 25 in steps of 1
After free(), program break is:  0x8891000
```

## Ex. 7-2

**Question**

(Advanced) Implement malloc() and free().

**Answer**

See *malloc_free_imp.c*
