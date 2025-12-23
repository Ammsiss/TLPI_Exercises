## Syscalls

`stat(2)` - retrieves information about a file  
`lstat(2)` - doesn't follow links  
`fstat(2)` - specify file with a fd

`utime(2)` - explicitly change file timestamps  
`utimes(2)`  
`futimes(3)`  
`utimensat(2)`  
`futimens(2)`

`chown(2)` - set the gid and the uid of a file  
`lchown(2)`  
`fchown(2)`

`access(2)` - checks file accessibility  
See *Borisov, 2005*, as to why access(2) should never be used.

`umask(2)` - set the current processes umask value  
`chmod(2)` - sets the permission bits on a file

## Tables

**Table 15-2**: various functions effects on timestamps.

## TODO

- Find out why *t_utime.c* does not independently update atime
  when `atime == mtime` even with when mounted with
  *strictatime*. (Perhaps it is the `FS_NOATIME_FL` inode flag).

- Review **Section 15.4.7** exploit prevention via clearing of
  set gid function.

- Write a c program for **Exersize 15-1.b**

- Review TLPI answers repo for more robust solution of exersize
  3.

- Review *sysdeps/posix/euidaccess.c* which is a GNU C
  libarary implementation of **Exersise 15-4**.

- Make a program that takes a file and a list of dependencies
  (likemake) and tells you if any of the pre-reqs are out of
  date. The checking algorithm can work by first seeing if all
  pre-reqs exist then determining if its been modified at any
  time later then the last modificaiton of the target.
