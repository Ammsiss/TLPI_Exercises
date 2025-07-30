## Syscalls

- **stat(2)** -> Retrieves information about a file
    (See t_stat.c, lstat(2), fstat(2) and stat(1))

- **utime(2)** -> Explicitly change file timestamps
    (See t_utime.c, utimes(2), futimes(3), utimensat(2) and futimens(2))

- **chown(2)** -> Set the gid and the uid of a file
    (See t_chown.c, lchown(2) and fchown(2))

- **access(2)** -> Checks file accessibility based on real uid and gid of the process
    (See *Borisov, 2005*, as to why access(2) should never be used.)

- **umask(2)** -> Set the current processes umask value
    (See t_umask.c and umask(1))

- **chmod(2)** -> Sets the permission bits on a file
    (See t_chmod.c and chmod(1))

## Tables

- See **Table 15-2** for the effect various functions have on timestamps.

## TODO

- Find out why *t_utime.c* does not independently update atime when
  atime == mtime even with when mounted with the *strictatime*.
  (Perhaps it is the FS_NOATIME_FL inode flag).

- Review 15.4.7 exploit prevention via clearing of set gid function.

- Write a c program for exersize 15-1. b)

- Review TLPI answers repo for more robust solution of exersize 3.

- Review ```sysdeps/posix/euidaccess.c``` which is a GNU C libarary implementation
  of exersise 15-4.
