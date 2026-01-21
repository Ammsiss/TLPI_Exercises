## Syscalls

### I/O Multiplexing API

`select()` -> Advises fd events
`poll()` -> Advises fd events

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

## Listings

**Listing 63-1** (*t_select.c*): Using `select()` to monitor
multiple file descriptors

**Listing 63-2** (*poll_pipes.c*): Using `poll()` to monitor
multiple file descriptors

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

## More Info

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
