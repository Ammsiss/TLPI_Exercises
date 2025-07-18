For a great visual on libc VS kernal buffering see Figure 13-1, p244.

### synchronized I/O *FILE* integrity completion:
File data and all meta data flushed to disk.

### synchronized I/O *DATA* integrity completion:
File data and all meta data required for writing flushed to disk.

**fsync()** flushes the specified fd according to the requirements
of synchronized I/O *FILE* integrity completion.

**fdatasync()** flushes the specified fd according to the requirements
of synchronized I/O *DATA* integrity completion.

O_SYNC  -> fsync()              (after every call to write())
O_DSYNC -> fdatasync()          (after every call to write())

O_RSYNC + O_SYNC -> fsync()     (after every call to read())
O_DSYNC + O_SYNC -> fdatasync() (after every call to read())

## TODO:

- Remove single character read calls from 01-ex.c in favour of a
  buffered approach.
