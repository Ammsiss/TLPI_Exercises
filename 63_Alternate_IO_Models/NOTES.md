## Syscalls

### I/O Multiplexing API

`select()` -> Advises fd events
`poll()` -> Advises fd events

`pselect()` -> Unmask signal while blocking
`ppoll()` -> Unmask signal while blocking

`FD_ZERO()` -> Initialize an fd set
`FD_SET()` -> Add an fd to a set
`FD_CLR()` -> Remove an fd from a set
`FD_ISSET()` -> Query if an fd is in a set

`FD_SETSIZE` -> Max numeric fd value in a set

**poll flags of interest:**

`POLLIN` -> Normal data can be read
`POLLOUT` -> Normal data can be written
`POLLPRI` -> Prio-data can be written
`POLLRDHUP` -> Shutdown on peer socket
`POLLHUP` -> A hangup has occurred
`POLLERR` -> An error has occurred

`epoll_create()` -> Creates a new epoll instance
`epoll_ctl()` -> Modified an epoll interest list
`epoll_wait()` -> Return info from an fd(s) in the ready list

## Listings

**Listing 63-1** (*t_select.c*): Using `select()` to monitor
multiple file descriptors

**Listing 63-2** (*poll_pipes.c*): Using `poll()` to monitor
multiple file descriptors

**Listing 63-3** (*demo_sigio.c*): Using signal-driven I/O on a
terminal

## Tables

**Table 63-1**: Use of level-triggered and edge-triggered
notification models

**Table 63-2**: Bit-mask values for events and revents fields of
the pollfd structure

**Table 63-3**: `select()` and `poll()` indications for
terminals and pseudoterminals

**Table 63-4**: `select()` and `poll()` indications for the read
end of a pipe or FIFO

**Table 63-5**: `select()` and `poll()` indications for the
write end of a pipe or FIFO

**Table 63-6**: `select()` and `poll()` indications for sockets

**Table 63-7**: `si_code` and `si_band` values in the
`siginfo_t` structure for “I/O possible” events

## More Info

Stevens et al., 2004 describes I/O multiplexing and
signal-driven I/O, with particular emphasis on the use of these
mechanisms with sockets. Gammo et al, 2004 is a paper
comparing the performance of select(), poll(), and epoll. A
particularly interesting online resource is at
http://www.kegel.com/c10k.html. Written by Dan Kegel, and
entitled “The C10K problem,” this web page explores the issues
facing developers of web servers designed to simultaneously
serve tens of thousands of clients. The web page includes a host
of links to related information.

## TODO

- [ ] Build a small epoll-based network file server that handles
  multiple clients with non-blocking IO. The project exercises
  core epoll patterns like readiness-driven reads and writes,
  per-connection state, and backpressure. It makes edge vs level
  triggering and partial IO concrete instead of theoretical.
  Small scope, high signal for learning event-driven network
  programming.
- [ ] Write an abstract software layer for monitoring file
  descriptor events. This layer should allow portable programs
  to employ epoll (or a similar API) on systems that provide it,
  and fall back to the use of select() or poll() on other
  systems.
- [ ] Modify *echo_sv.c* so that it is concurrent instead of
  iterative.
