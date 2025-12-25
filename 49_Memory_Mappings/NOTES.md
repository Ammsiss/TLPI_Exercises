## Syscalls

mmap -> Create a memory mapping  
munmap -> Remove a memory mapping  
msync -> Flush changes in the kernel page cache of a mmap to
disk  
mremap -> Resize a memory mapping  
remap_file_pages -> Change the order of memory mapping -> file
pages  

## Tables

**Table 49-1**: Purposes of various types of memory mappings
**Table 49-2**: Memory protection values
**Table 49-4**: Handling of swap space reservation during mmap()

**Figure 49-1**: Overview of memory-mapped file
**Figure 49-2**: Two processes with a shared mapping of the same
region of a file
**Figure 49-3**: Memory mapping whose length is not a multiple
of the system page size
**Figure 49-5**: A nonlinear file mapping

## More Info

- To obtain system page size: ```ps = sysconf(_SC_PAGESIZE);```

- Theres any idea of "memory quality" between file backed memory
  mappings and anonomous ones. File backed memory mappings are
  mroe flexible and higher quality because the kernel can use
  its optimzation tehnciques to free it from memory when it
  decides too and it can always get it back from the original
  file when needed. This is the case in a private file mapping
  or with any pages in a shared file mapping that have NOT been
  modified. An anonomous memory mapping is lower quailty and
  "dirty" because the kernel can never reclaim as it does not
  have the original file to bring it back if needed.

- For linux with a unified virtual memory system,
  synchronization of shared file mappings is really simple. The
  mapping you get is the same as the kernel page cache therefore
  writes are immediately seen by other processes and reads are
  guaranteed to be the most up to date version (minus some rare
  race conditions). In this case msync()'s only purpose is to
  flush the cache to disk.

- Information about the implementation of mmap() on Linux can be
  found in Bovet & Cesati, 2005. Information about the
  implementation of mmap() on other UNIX systems can be found in
  McKusick et al., 1996 (BSD), Goodheart & Cox, 1994 (System V
  Release 4), and Vahalia, 1996 (System V Release 4).

## TODO

- erratta for figure 49-4 ? specifies 0 in the mmap call when
  should be NULL as well as the explanaiton of that figure being
  quite confusing.

- Make a read only text viewer that utilizes a shared memory
  mapping to load the file. This automatically gives you lazy
  loading via the kernels copy-on-write mechanism. You can
  optionally also add "memory- poking" where you change a single
  byte worth of information as that would not require shifting
  the file contents. You can use this to have a efficient viewer
  for the chat.html or conversations.json chatgpt export that
  is 270MB large. Good optimization test.

- Test out the semantics of opening a private file mapping that
  other processes are modifying while you have the mapping open.
  If you open a mapping, read a section, then another process
  modifies that section and you read it again, is it updated
  since the first time you read it?

- Come back to the last exersize (non-linear mapping)
