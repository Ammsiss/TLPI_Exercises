# Chapter 44: Pipes and FIFOs
## Ex. 44-1

**Question**

Write a program that uses two pipes to enable bidirectional communication
between a parent and child process. The parent process should loop reading a
block of text from standard input and use one of the pipes to send the text to the
child, which converts it to uppercase and sends it back to the parent via the other
pipe. The parent reads the data coming back from the child and echoes it on
standard output before continuing around the loop once more.

**Answer**

See *bidir_pipe.c*

## Ex. 44-2

**Question**

Implement popen() and pclose(). Although these functions are simplified by not
requiring the signal handling employed in the implementation of system()
(Section 27.7), you will need to be careful to correctly bind the pipe ends to file
streams in each process, and to ensure that all unused descriptors referring to the
pipe ends are closed.

Since children created by multiple calls to popen() may be running at one time,
you will need to maintain a data structure that associates the file stream pointers
allocated by popen() with the corresponding child process IDs. (If using an array
for this purpose, the value returned by the fileno() function, which obtains the
file descriptor corresponding to a file stream, can be used to index the array.)

Obtaining the correct process ID from this structure will allow pclose() to select
the child upon which to wait. This structure will also assist with the SUSv3
requirement that any still-open file streams created by earlier calls to popen() 
must be closed in the new child process.

**Answer**

See *popen_close_imp.c*

## Ex. 44-3

**Question**

The server in Listing 44-7 (fifo_seqnum_server.c) always starts assigning sequence
numbers from 0 each time it is started. Modify the program to use a backup file
that is updated each time a sequence number is assigned. (The open() O_SYNC flag,
described in Section 4.3.1, may be useful.) At startup, the program should check
for the existence of this file, and if it is present, use the value it contains to
initialize the sequence number. If the backup file can’t be found on startup, the
program should create a new file and start assigning sequence numbers beginning at
0. (An alternative to this technique would be to use memory-mapped files, described
in Chapter 49.)

**Answer**

See relevant code in *fifo_seqnum_server.c*

## Ex. 44-4

**Question**

Add code to the server in Listing 44-7 (fifo_seqnum_server.c) so that if the program
receives the SIGINT or SIGTERM signals, it removes the server FIFO and terminates.

**Answer**

See relevant code in *fifo_seqnum_server.c*

## Ex. 44-5

**Question**

The server in Listing 44-7 (fifo_seqnum_server.c) performs a second O_WRONLY open of
the FIFO so that it never sees end-of-file when reading from the reading descriptor
(serverFd) of the FIFO. Instead of doing this, an alternative approach could be tried:
whenever the server sees end-of-file on the reading descriptor, it closes the
descriptor, and then once more opens the FIFO for reading. (This open would
block until the next client opened the FIFO for writing.) What is wrong with this
approach?

**Answer**

This would be a problem as it is a race condition. During the time it takes for the
server to close and open its FIFO for reading a client could both open the server
FIFO for writing (because the server is running after all) and attempt a write. This
would have the undesired effect of sending the client the SIGPIPE signal which has
the default disposition of killing the process.

## Ex. 44-6

**Question**

The server in Listing 44-7 (fifo_seqnum_server.c) assumes that the client process is
well behaved. If a misbehaving client created a client FIFO and sent a request to the
server, but did not open its FIFO, then the server’s attempt to open the client FIFO
would block, and other client’s requests would be indefinitely delayed. (If done
maliciously, this would constitute a denial-of-service attack.) Devise a scheme to deal
with this problem. Extend the server (and possibly the client in Listing 44-8)
accordingly.

**Answer**

See relevant code in *fifo_seqnum_server.c* as well as *malicious_client.c*

## Ex. 44-7

**Question**

Write programs to verify the operation of nonblocking opens and nonblocking I/O
on FIFOs (see Section 44.9).

**Answer**

See *nonblock_open_test.c*

This program takes command line arguments specifying the flags sent to open as
well as if the other side of the pipe should be open or not during the call to
open. If the open calls succeeds it then tests the non blocking reading or writing
operations (depending on the command line args) of the FIFO with that fd.

Heres an example output:

```bash
$ ./nonblock_open_test.bin w o

Attempting to open FIFO with the following setup:
    Flags: O_NONBLOCK, O_WRONLY
    Other end open? Yes

Success!

We will now test nonblocking write operations with your file descriptor. (PIPE_BUF=4096)
Keep other end open? [y/n]
> y
Enter number of bytes that should be in the pipe [0,65536]
> 65535
Enter number of bytes to be written to the pipe [0,70000]
> 4097

Attempting write...

1 bytes written
```

This output demonstrates that if a write to a pipe is MORE then PIPE_BUF (meaning
that the kernel does not have the requirement to make the write atomic) then as many
bytes as the pipe can store will be written.

If you did not have the O_NONBLOCK flag enabled in this situation, 1 byte would
immediately be written but the write call would then block until sufficient space was
avaialble to write the remaining bytes.
