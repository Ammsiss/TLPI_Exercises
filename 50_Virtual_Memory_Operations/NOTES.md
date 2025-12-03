## Syscalls

mprotect -> change the "protection" of virutal memory
mlock -> lock a region of memory
munlock -> unlock a region of memory
mlockall -> lock all memory
munlockall -> unlock all memory
mincore -> Report which pages in an address range are stored in ram
madvise -> Advise kernal about likely usage of memory region

## Tables
## More Info

- Locking is done in whole pages so a mlock(2000, 4000) call
(assuming a system page size of 4096) would lock from bytes
0-8191

## TODO
