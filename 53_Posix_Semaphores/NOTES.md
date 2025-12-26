## Syscalls

`sem_open()` - Open a named semaphore

`sem_close()` - Close in memory association

`sem_unlink()` - Unlink semaphore from filesystem

`sem_wait()` - Decrement a semaphore  
`sem_trywait()` - Fail if semaphore is 0  
`sem_timedwait()` Block for a time while semaphore is 0

`sem_post()` - Increment a semaphore

`sem_init()` - Initialize an unamed semaphore  
`sem_destroy()` - Destroy an unamed semaphore

## Listings

**Listing 53-1** (*psem_create.c*): Using `sem_open()` to open
or create a POSIX named semaphore

**Listing 53-2** (*psem_unlink.c*): Using `sem_unlink()` to unlink a POSIX named
semaphore

**Listing 53-3** (*psem_wait.c*): Using `sem_wait()` to
decrement a POSIX semaphore

**Listing 53-4** (*psem_post.c*): Using `sem_post()` to
increment a POSIX semaphore

**Listing 53-5** (*psem_getvalue.c*): Using `sem_getvalue()` to
retrieve the value of a POSIX semaphore

**Listing 53-6** (*thread_incr_psem.c*): Using a POSIX unnamed
semaphore to protect access to a global variable

## More Info

Named semaphores on Linux reside in */dev/shm/sem.<name>*

**SEM_NSEMS_MAX**: This is the maximum number of POSIX
semaphores that a process may have. SUSv3 requires that this
limit be at least 256. On Linux, the number of POSIX semaphores
is effectively limited only by available memory.

**SEM_VALUE_MAX**: This is the maximum value that a POSIX
semaphore may reach. Semaphores may assume any value from 0 up
to this limit. SUSv3 requires this limit to be at least 32,767;
the Linux implementation allows values up to `INT_MAX`
(2,147,483,647 on Linux/x86-32).

**Further Information**: *Stevens, 1999* provides an alternative
presentation of POSIX semaphores and shows user-space
implementations using various other IPC mechanisms (FIFOs,
memory-mapped files, and System V semaphores). *Butenhof, 1996*
describes the use of POSIX semaphores in multithreaded
applications.

## TODO

- Modify your thread safe binary tree implementation to use
  POSIX semaphores instead of mutexes.
