# Chapter 31: Threads: Thread Safety and Per-Thread Storage
## Ex. 31-1

**Question**

Implement a function, one_time_init(control, init), that
performs the equivalent of pthread_once(). The control
argument should be a pointer to a statically allocated
structure containing a Boolean variable and a mutex.
The Boolean variable indicates whether the function init
has already been called, and the mutex controls access to
that variable.

To keep the implementation simple, you can
ignore possibilities such as init() failing or being canceled
when first called from a thread (i.e., it is not necessary to
devise a scheme whereby, if such an event occurs, the next
thread that calls one_time_init() reattempts the call to init()).

**Answer**

See *one_time_call.c*

## Ex. 31-2

**Question**

Use thread-specific data to write thread-safe versions of dirname()
and basename() (Section 18.14).

**Answer**

See *dirbase_name_tls.c* and *dirbase_name_tsd.c*
