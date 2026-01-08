## Syscalls

`htons()` - Convert short to network byte order  
`htonl()` - Convert long to network byte order  
`ntohs()` - Convert short to host byte order  
`ntohl()` - Convert long to host byte order

`inet_pton()` - Converts IP4/6 from presentation -> network  
`inet_ntop()` - Converts IP4/6 from network -> presentation

`getaddrinfo()` - Get socket structure from name  
`freeaddrinfo()` - Free structure returned by `getaddrinfo()`
`gai_strerror()` - Convert `getaddrinfo()` error code to string

`getnameinfo()` - Get name from socket structure

**OBSOLETE**

`inet_aton()`  
`inet_ntoa()`

`gethostbyname()`  
`gethostbyaddr()`

`herror()`  
`hstrerror()`

`getservbyname()`  
`getservbyport()`

## Listings

**Listing 59-1** (*read_line.c*) : Reading data a line at a time

**Listing 59-2** (*i6d_ucase.h*): Header file used by
`i6d_ucase_sv.c` and `i6d_ucase_cl.c`  
**Listing 59-3** (*i6d_ucase_sv.c*): IPv6 case-conversion server
using datagram sockets  
**Listing 59-4** (*i6d_ucase_cl.c*): IPv6 case-conversion client
using datagram sockets

**Listing 59-5** (*is_seqnum.h*): Header file used by
*is_seqnum_sv.c* and *is_seqnum_cl.c*  
**Listing 59-6** (*is_seqnum_sv.c*): An iterative server that
uses a stream socket to communicate with clients  
**Listing 59-7** (*is_seqnum_cl.c*): A client that uses stream
sockets

**Listing 59-8** (*inet_sockets.h*): Header file for
*inet_sockets.c*  
**Listing 59-9** (*inet_sockets.c*): An Internet domain sockets
library

**Listing 59-10**: Using `gethostbyname()` to retrieve host
information

## Tables

**Figure 59-1**: Big-endian and little-endian byte order for
2-byte and 4-byte integers

**Figure 59-2**: A subset of the DNS hierarchy

**Figure 59-3**: Structures allocated and returned by
`getaddrinfo()`

**Table 59-1**: Error returns for `getaddrinfo()` and
`getnameinfo()`

## More Info

The process of putting data into a standard format for
transmission across a network is referred to as *marshalling*.

```c
#define INET_ADDRSTRLEN 16 /* Maximum IPv4 dotted-decimal string */
#define INET6_ADDRSTRLEN 46 /* Maximum IPv6 hexadecimal string */
```

`NI_MAXHOST` indicates the maximum size, in bytes, for a returned
hostname string. It is defined as 1025.

`NI_MAXSERV` indicates the maximum size, in bytes, for a
returned service name string. It is defined as 32.

**Further Information**:

- The key book on network programming with the sockets API is
  *Stevens at al., 2004*. *Snader, 2000* adds some useful
  guidelines on sockets programming.
- *Stevens, 1994* and *Wright & Stevens, 1995* describe TCP/IP
  in detail. *Comer, 2000*, *Comer & Stevens, 1999*, *Comer &
  Stevens, 2000*, *Kozierok, 2005*, and *Goralksi, 2009* also
  provide good coverage of the same material.
- *Tanenbaum, 2002* provides general background on computer
  networks.
- *Herbert, 2004* describes the details of the Linux 2.6 TCP/IP
  stack.
- The GNU C library manual (online at [http://www.gnu.org/]) has
  an extensive dis- cussion of the sockets API.
- The IBM Redbook, TCP/IP Tutorial and Technical Overview,
  provides lengthy coverage of networking concepts, TCP/IP
  internals, the sockets API, and a host of related topics. It
  is freely downloadable from [http://www.redbooks.ibm.com/].
- *Gont, 2008* and *Gont, 2009b* provide security assessments of
  IPv4 and TCP.
- The Usenet newsgroup comp.protocols.tcp-ip is dedicated to
  questions related to the TCP/IP networking protocols. 1236
  Chapter 59
- *Sarolahti & Kuznetsov, 2002* describes congestion control and
  other details of the Linux TCP implementation.
- Linux-specific information can be found in the following
  manual pages: `socket(7),` `ip(7),` `raw(7),` `tcp(7),`
  `udp(7),` and `packet(7).`
- See also the RFC list in **Section 58.7**.

## TODO

- Make the `read_line_buf()` function read 1 more then
`MAX_MSG_SIZE` so that it does not use the swap buffer when
messages are exactly `MAX_MSG_SIZE` long.
    - Think about testing `read_line_buf()` its a good function
      to practice writing tests. For example 1..N..1, all ways
      of partitioning short lines (perhaps 1-50), random chunk
      sizes, adversial chunking, performance, etc. It will be
      useful to make a fake fd so that you can control `read()`
      perhaps a function pointer or `socketpair()`
