# Chapter 63: Alternate I/O Models

## Ex. 63-1

**Question**

Modify the program in **Listing 63-2** (*poll_pipes.c*) to use
`select()` instead of `poll()`.

**Answer**

See *pipe_with_select.c*

## Ex. 63-2

**Question**

Write an echo server (see **Section 60.2** and **Section 60.3**)
that handles both TCP and UDP clients. To do this, the server
must create both a listening TCP socket and a UDP socket, and
then monitor both sockets using one of the techniques described
in this chapter.

**Answer**

See *echo_sv.c*, *echo_udp_cl.c* and *echo_tcp_cl.c*

## Ex. 63-3

**Question**

**Section 63.5** noted that `select()` can’t be used to wait on
both signals and file descriptors, and described a solution
using a signal handler and a pipe. A related problem exists when
a program needs to wait for input on both a file descriptor and
a System V message queue (since System V message queues don’t
use file descriptors). One solution is to fork a separate child
process that copies each message from the queue to a pipe
included among the file descriptors monitored by the parent.
Write a program that uses this scheme with `select()` to monitor
input from both the terminal and a message queue.

**Answer**

Not doing System V questions

## Ex. 63-4

**Question**

The last step of the description of the self-pipe technique in
**Section 63.5.2** stated that the program should first drain
the pipe, and then perform any actions that should be taken in
response to the signal. What might happen if these substeps were
reversed?

**Answer**

If you respond first there is a race condition between the time
you finish and the time you start draining the pipes becuase
another signal could come in and subsequently be drained as
well.

## Ex. 63-5

**Question**

Modify the program in **Listing 63-9** (*self_pipe.c*) to use
`poll()` instead of `select()`.

**Answer**

See *selfpipe_with_select.c*

## Ex. 63-6

**Question**

Write a program that uses `epoll_create()` to create an epoll
instance and then immediately waits on the returned file
descriptor using `epoll_wait()`. When, as in this case,
`epoll_wait()` is given an epoll file descriptor with an empty
interest list, what happens? Why might this be useful?

**Answer**

Even if the interest list is empty `epoll_wait()` blocks. This
can be useful in multi-threaded applications.

## Ex. 63-7

**Question**

Suppose we have an epoll file descriptor that is monitoring
multiple file descriptors, all of which are always ready. If we
perform a series of `epoll_wait()` calls in which maxevents is
much smaller than the number of ready file descriptors (e.g.,
maxevents is 1), without performing all possible I/O on the
ready descriptors between calls, what descriptor(s) does
`epoll_wait()` return in each call? Write a program to determine
the answer. (For the purposes of this experiment, it suffices to
perform no I/O between the `epoll_wait()` system calls.) Why
might this behavior be useful?

**Answer**

It seemed to loop over them, reporting each one in the order
that I added them with epoll_ctl(). This is useful because it
does not only process input from 1 descriptor consequently
starving the others.

See *epoll_priority_test.c*

```bash
$ ./epoll_priority_test.bin
```

```
fd = 4
fd = 6
fd = 8
fd = 10
fd = 12
fd = 4
fd = 6
```

## Ex. 63-8

**Question**

Modify the program in **Listing 63-3** (*demo_sigio.c*) to use a
realtime signal instead of `SIGIO`. Modify the signal handler to
accept a `siginfo_t` argument and display the values of the
`si_fd` and `si_code` fields of this structure.

**Answer**

See *rtsigio.c* and *sigio_rts_demo.c*
