## Syscalls

flock -> place a lock on a file
fcntl -> can be used to place a lock with a superset of flock features

## Tables

**Figure 55-1**: Two processes updating a file at the same time without synchronization

## More Info

See *i_fcntl_locking.c" for a good way to parse an input command using sscanf and strchr

## TODO

- Remove the sleep sync in *lock_starve.c* in favour of an actual IPC mechanism.
- Finish: ex 8,10
