## Syscalls

**fcntl(2)** -> Perform a range of control operations on an open fd

**dup(2)** -> Duplicate a fd
**dup2(2)** -> Duplicates an fd with a specifiable fd number
**dup3(2)** -> Same as *dup2()* but supports an additional flag argument

**pread(2)** -> Read a specific offset of a fd, Atomically
**pwrite(2)** -> Write to a specific offset of a fd, Atomically

**readv(2)** -> Perform a 'scatter gather' read
**writev(2)** -> Perform a 'scatter gather' write
**preadv(2)** -> Same as *readv()* but at a specific offset, Atomically
**pwritev(2)** -> Same as *writev()* but at a specific offset, Atomically

**truncate(2)** -> Set the size of a file specified by a string path
**ftruncate(2)** -> Set the size of a file specified by a fd

**mkstemp(3)** -> Create and open a unique filename based on a tempalte
**tmpfile(3)** -> Similar to *mkstemp(2)* except C11 and returns a FILE*

## More Info

## Tables

See **Figure 5-2** for a relationship between file descriptors, open file
descriptions, and i-nodes.
