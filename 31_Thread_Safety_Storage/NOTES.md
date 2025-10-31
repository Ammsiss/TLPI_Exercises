## Syscalls

**pthread_once(3)** -> Ensures a user supplied init function is called only once across threads

**pthread_key_create(3)** -> Creates a new thread-specific data key

**pthread_setspecific(3)** -> Save a data structure and associate it with a key
**pthread_getspecific(3)** -> Return a data structure previously associated with a key

## Tables

**Table 31-1**: Functions that SUSv3 does not require to be thread-safe
**Figure 31-2**: Thread-specific data (TSD) provides per-thread storage for a function
**Figure 31-3**: Data structure used to implement thread-specific data (TSD) pointers

## More Info
## TODO

- Update dirbase_name_tsd.c so that it uses 1 key and a custom data structure for
each function that needs TSD.
