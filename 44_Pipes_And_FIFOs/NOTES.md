## Syscalls

pipe -> Create a pipe!
mkfifi -> Create a fifo!

## Tables

**Figure 44-1**: Using a pipe to connect two processes
**Figure 44-2**: Process file descriptors after creating a pipe
**Figure 44-3**: Setting up a pipe to transfer data from a parent to a child
**Figure 44-4**: Overview of process relationships and pipe usage for popen()
**Figure 44-5**: Using a FIFO and tee(1) to create a dual pipeline

**Table 44-1**: Semantics of open() for a FIFO
**Table 44-2**: Semantics of reading n bytes from a pipe or FIFO containing p bytes
**Table 44-3**: Semantics of writing n bytes to a pipe or FIFO

## More Info

```c
/*
    Why we have the if statement? If for example STDIN and STDOUT were already
    closed, then the subsequent pipe call would have used those two descriptors
    for its own fd's. This could mean that pfd[1] == STDOUT_FILENO, and if this
    is the case then the subsequent dup2 call would do nothing and you would
    permanently close pfd[1].

    You should also use dup2 over dup because of the same issue being that dup
    would potentially use a lower fd number then STDOUT (if thats the one your
    replacing) if it is already closed prior to the dup call.
*/

if (pfd[1] != STDOUT_FILENO) {
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
}
```

## TODO

- Recreate the client server fifo program using a *concurrent* server model.
IE one that makes use of a separate child process or thread to handle each
incoming request. This is also good for the malicious client program as the
retries won't affect other clients.

- Clean up code for *nonblock_open_text.c* especially that redundant write
