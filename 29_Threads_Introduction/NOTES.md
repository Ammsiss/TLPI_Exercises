## Syscalls

**pthread_create(3)** -> Create a new thread

**pthread_exit(3)** -> Terminate the calling thread

**pthread_self(3)** -> Return Thread ID (TID) of calling thread

**pthread_equal(3)** -> Equality test 2 TID's

**pthread_join(3)** -> Wait for a thread to terminate / *join* a thread

**pthread_detach(3)** -> Detach a thread and make it un-joinable

## Tables

**Table 29-1**: Pthreads data types

## More Info

- Programs that use the **Pthreads API** must be compiled wit hthe *cc -pthread* option.

- *Butenhof, 1996* provides an exposition of Pthreads that is both readable and thorough.

- *Robbins & Robbins, 2003* also provides good coverage of Pthreads.

- *Tanenbaum, 2007* provides a more theoretical introduction to thread concepts, covering
topics such as mutexes, critical regions, conditional variables, and deadlock detec-
tion and avoidance.

- *Vahalia, 1996* provides background on the implementation of threads.

## TODO
