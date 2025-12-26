## Syscalls

`socket()` - Creates a new socket  
`bind()` - Bind a socket to an address

`listen()` - Mark a socket as passive  
`accept()` - Accepts an incoming connection  
`connect()` - Connect to a passive socket

`recvfrom()` - Receive a datagram socket  
`sendto()` - Send a datagram socket

## Tables

**Table 56-1**: Socket domains

**Table 56-2**: Socket types and their properties

**Figure 56-1**: Overview of system calls used with stream
sockets

**Figure 56-2**: A pending socket connection

**Figure 56-3**: UNIX domain stream sockets provide a
bidirectional communication channel

**Figure 56-4**: Overview of system calls used with datagram
sockets

## More Info

`SOMAXCONN` in <sys/socket.h> for the max limit of pending
`connect()` connections. Linux default is 128. See
Linux-specific `/proc/sys/net/core/somaxconn` in order to modify
this limit.

**Further Information**: Refer to the sources of further
information listed in **Section 59.15**

See [socket(7)]() for general information about the sockets api.

## TODO
