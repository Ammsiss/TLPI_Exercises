## Syscalls

`shm_open()` - Opens a shared memory object
`shm_unlink()` - Removes kernel persistence

## Listings

**Listing 54-1** (*pshm_create.c*): Creating a POSIX shared
memory object

**Listing 54-2** (*pshm_write.c*): Copying data into a POSIX
shared memory object

**Listing 54-3** (*pshm_read.c*): Copying data from a POSIX
shared memory object

**Listing 54-4** (*pshm_unlink.c*): Using `shm_unlink()` to
unlink a POSIX shared memory object

## Tables

**Table 54-1**: Bit values for the `shm_open()` oflag argument

## More Info

See **Section 54.5** for Comparisons Between Shared Memory APIs

## TODO
