## Syscalls

flock -> place a lock on a file
fcntl -> can be used to place a lock with a superset of flock features

## Tables

**Figure 55-1**: Two processes updating a file at the same time without synchronization
**Table 55-1**: Values for the operation argument of flock()
**Table 55-2**: Compatibility of flock() locking types
**Table 55-3**: Lock types for fcntl() locking
**Figure 55-2**: Using record locks to synchronize access to the same region of a file
**Figure 55-3**: Splitting of an existing read lock by a write lock by the same process
**Figure 55-4**: Deadlock when two processes deny each other’s lock requests
**Figure 55-5**: State of granted and queued lock requests while running i_fcntl_locking.c
**Figure 55-6**: Example of a record lock list for a single file
**Figure 55-7**: Deadlock when mandatory locking is in force

## More Info

See *i_fcntl_locking.c" for a good way to parse an input command using sscanf and strchr

See *fcntl(2)* and search for strings 'advisory locks', and 'mandatory locks'. For
OFD (Open file description) locks search for '(non'

## TODO

- Remove the sleep sync in *lock_starve.c* in favour of an actual IPC mechanism.
- Finish: ex 8,10
