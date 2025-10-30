## Syscalls

```c
pthread_mutex_lock(3);      /* Lock a mutex, blocking if already locked */
pthread_mutex_trylock(3);   /* Lock a mutex, failing if already locked */
pthread_mutex_timedlock(3); /* Lock a mutex, failing after a specified time */

pthread_mutex_unlock(3);    /* Unlock a mutex */

pthread_mutex_init(3);      /* Dynamically allocate a mutex */
pthread_mutex_destroy(3);   /* Destroy a dynamically allocated mutex */

pthread_cond_signal(3);     /* Signal a specified condition varaible */
pthread_cond_broadcast(3);  /* Signal a specified condition varaible */

pthread_cond_wait(3);       /* Block a thread until a condition varaible is signaled */
pthread_cond_timedwait(3);  /* Same as pthread_cond_wait() but timed */

pthread_cond_init(3);       /* Dynamically allocatea a condition variable */
pthread_cond_destroy(3);    /* Destroy a dynamically allocated condition variable */
```

## Tables

**Figure 30-1**: Two threads incrementing a global variable without synchronization
**Figure 30-2**: Using a mutex to protect a critical section
**Figure 30-3**: A deadlock when two threads lock two mutexes

## More Info


## TODO
