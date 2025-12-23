## Syscalls

`pthread_mutex_lock(3)`      - blocks if already locked  
`pthread_mutex_trylock(3)`   - fails if already locked  
`pthread_mutex_timedlock(3)` - fails after a specified time

`pthread_mutex_unlock(3)`

`pthread_mutex_init(3)`    - dyn-alloc a mutex  
`pthread_mutex_destroy(3)` - dealloc a dyn-alloc mutex

`pthread_cond_signal(3)`    - wake one thread on a cond-var  
`pthread_cond_broadcast(3)` - wake all threads on a cond-var

`pthread_cond_wait(3)`      - block until a cond-var is signaled  
`pthread_cond_timedwait(3)` - same but timed

`pthread_cond_init(3)`    - dyn-alloc a cond-var  
`pthread_cond_destroy(3)` - dealloc a dyn-alloc cond-var

## Tables

**Figure 30-1**: Two threads incrementing a global variable
without synchronization  
**Figure 30-2**: Using a mutex to protect a critical section  
**Figure 30-3**: A deadlock when two threads lock two mutexes

## More Info

## TODO
