## Syscalls

`shutdown()` -> Close just one half of a socket

`recv()` -> Like read but with socket specific functionality
`send()` -> Like write but with socket specific functionality

`sendmsg() and recvmsg()` -> All features of recv and send + rw

`sendfile()` -> Send a file out a socket bypassing userspace

`getsockname()` -> Get local sockaddr of a socket
`getpeername()` -> Get peer sockaddr of a socket

`getsockopt()` -> Get socket options
`setsockopt()` -> Set socket options

## Listings

**Listing 61-1** (*rdwrn.c*): Implementation of `readn()` and
`writen()`

**Listing 61-2** (*is_echo_cl.c*): A client for the echo service

**Listing 61-3** (*socknames.c*): Using `getsockname()` and
`getpeername()`

## Tables

**Figure 61-1**: Transferring the contents of a file to a socket

**Figure 61-2**: Format of a TCP segment

**Figure 61-3**: Acknowledgements in TCP

**Figure 61-4**: TCP state transition diagram

**Figure 61-5**: Three-way handshake for TCP connection establishment

**Figure 61-6**: TCP connection termination

**Table 61-1**: Options for the netstat command

## More Info

Refer to **Section 59.15** further info section.

An extensive discussion covering most standard socket options is
provided in [Stevens et al., 2004].

See the **tcp(7)**, **udp(7)**, **ip(7)**, **socket(7)**, and
**unix(7)** manual pages for additional Linux-specific details.

Given that TCP provides reliable delivery of data, while UDP
does not, an obvious question is, “Why use UDP at all?” The
answer to this question is covered at some length in
**Chapter 22** of [Stevens et al., 2004].

The use of out-of-band data is nowadays discouraged, and it may
be unreliable in some circumstances (see [Gont & Yourtchenko,
2009]).

See `tshark` and `ss`

SCTP is described in [Stewart & Xie, 2001], [Stevens et al.,
2004], and in RFCs 4960, 3257, and 3286.

SCTP is available on Linux since kernel 2.6. Further information
about this implementation can be found at
http://lksctp.sourceforge.net/. Throughout the preceding
chapters that describe the sock

Information about DCCP can be found at http://
www.read.cs.ucla.edu/dccp/ and RFCs 4336 and 4340.

## TODO

- See the solution to **Exersize 61-3**

- Come back to the final exersize after going through epoll
  chapter

- Find out if you made the mistake of not using `struct
  sockaddr_storage` in any other of the exersize programs.

- [ ] Modify the is_shell programs to explicitly call fork(),
  exec(), wait(), and exit(), intead of using system().
