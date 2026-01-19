# Chapter 61: Sockets: Advanced Topics

## Ex. 61-1

**Question**

Suppose that the program in **Listing 61-2** (*is_echo_cl.c*)
was modified so that, instead of using `fork()` to create two
processes that operate concurrently, it instead used just one
process that first copies its standard input to the socket and
then reads the server’s response. What problem might occur when
running this client? (Look at **Figure 58-8**, on page 1190.)

**Answer**

If you do not handle reading and writing from the socket
concurrently then you can run into the issue of the client
writing so much data that it fills both send and both receive
buffers. Subsequently a further read would block indefinitely.

## Ex. 61-2

**Question**

Implement `pipe()` in terms of `socketpair()`. Use `shutdown()`
to ensure that the resulting pipe is unidirectional.

**Answer**

See *pipe_imp.c*

## Ex. 61-3

**Question**

Implement a replacement for `sendfile()` using `read()`,
`write()`, and `lseek()`.

**Answer**

See *sendfile_imp.c*

## Ex. 61-4

**Question**

Write a program that uses `getsockname()` to show that, if we
call `listen()` on a TCP socket without first calling `bind()`,
the socket is assigned an ephemeral port number.

**Answer**

See *ephem_test.c*

After running the program a few times I got the following
output.

```bash
$ ./ephem_test.bin # 4 times
```

```
(0.0.0.0, 36129)
(0.0.0.0, 36967)
(0.0.0.0, 47681)
(0.0.0.0, 55747)
```

IP is wildcard and port is within the ephemeral range (usually
32768-60999).

## Ex. 61-5

**Question**

Write a client and a server that permit the client to execute
arbitrary shell commands on the server host. (If you don’t
implement any security mechanism in this application, you should
ensure that the server is operating under a user account where
it can do no damage if invoked by malicious users.) The client
should be executed with two command-line arguments:

```bash
$ ./is_shell_cl server-host 'some-shell-command'
```

After connecting to the server, the client sends the given
command to the server, and then closes its writing half of the
socket using `shutdown()`, so that the server sees end-of-file.
The server should handle each incoming connection in a separate
child process (i.e., a concurrent design). For each incoming
connection, the server should read the command from the socket
(until end-of-file), and then exec a shell to perform the
command. Here are a couple hints:

- See the implementation of `system()` in **Section 27.7** for
  an example of how to execute a shell command.
- By using `dup2()` to duplicate the socket on standard output
  and standard error, the execed command will automatically
  write to the socket.

**Answer**

See *is_shell_cl.c*, *is_shell_sv.c*, and *is_shell.h*

## Ex. 61-6

**Question**

**Section 61.13.1** noted that an alternative to out-of-band
data would be to create two socket connections between the
client and server: one for normal data and one for priority
data. Write client and server programs that implement this
framework. Here are a few hints:

- The server needs some way of knowing which two sockets belong
  to the same client. One way to do this is to have the client
  first create a listening socket using an ephemeral port (i.e.,
  binding to port 0). After obtaining the ephemeral port number
  of its listening socket (using `getsockname()`), the client
  connects its “normal” socket to the server’s listening socket
  and sends a message containing the port number of the client’s
  listening socket. The client then waits for the server to use
  the client’s listening socket to make a connection in the
  opposite direction for the “priority” socket. (The server can
  obtain the client’s IP address during the `accept()` of the
  normal connection.)
- Implement some type of security mechanism to prevent a rogue
  process from trying to connect to the client’s listening
  socket. To do this, the client could send a cookie (i.e., some
  type of unique message) to the server using the normal socket.
  The server would then return this cookie via the priority
  socket so that the client could verify it.
- In order to experiment with transmitting normal and priority
  data from the client to the server, you will need to code the
  server to multiplex the input from the two sockets using
  `select()` or `poll()` (described in **Section 63.2**).

**Answer**
