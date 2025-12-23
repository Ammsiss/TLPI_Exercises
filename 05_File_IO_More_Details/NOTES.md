## Syscalls

`fcntl(2)` - perform a range of control operations on an open fd

`dup(2)`  - duplicate a fd  
`dup2(2)` - duplicates an fd with a specifiable fd number  
`dup3(2)` - like `dup2()` but with an additional flag argument

`pread(2)`  - read a specific offset atomically  
`pwrite(2)` - write a specific offset atomically

`readv(2)`   - perform a 'scatter gather' read  
`preadv(2)`  - lets you specify a specific offset

`writev(2)`  - perform a 'scatter gather' write  
`pwritev(2)` - lets you speicfy a specific offset

`truncate(2)` - set the size of a file  
`ftruncate(2)` - lets you specify file by fd

`mkstemp(3)` - create a tempfile from a template  
`tmpfile(3)` - return a C-standard tempfile


## Tables

**Figure 5-2**: Relationship between file descriptors, open file
descriptions, and i-nodes.

## More Info
