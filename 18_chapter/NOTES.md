## Syscalls

**link(2)** -> Create a hardlink

**symlink(2)** -> Creates a symbolic link

**unlink(2)** -> Remove a hardlink (See *t_unlink.c*)
Note: Portable apps should avoid specifying symlinks for the *oldpath* arg.
See *t_unlink.c*

**rename(2)** -> Renames a file

**readlink(2)** -> Reads the content of a symbolic link (the stored path)

**mkdir(2)** -> Creates a new directory

**rmdir(2)** -> Removes a directory

**remove(2)** -> Removes a file or a directory

**opendir(2)** -> Returns a DIR (dir stream) that can be used to refer to a directory
See *fdopendir(2)*

**readdir(2)** -> Returns a directory entry from a DIR
Note: Returned structure is statically allocated. Directories are not returned
in order.

**rewinddir(3)** -> Moves a DIR back to the first directory

**closedir(3)** -> Closes and frees resources associated with a DIR

**dirfd(3)** -> Returns the fd associated with a given DIR

**nftw(3)** -> Walks a file tree and calls a function once for each file

**getcwd(3)** -> Return the calling proccesses current working directory
Note: Use PATH_MAX constant to define size (4096 bytes is the max)

**realpath(3)** -> Returns the absolute pathname from a path
Note: Use PATH_MAX for buffer size

**chdir(2)** -> Change current working directory
See *fchdir(2)*

**f<trad>at(2)** -> Various syscalls that perform analagously to
the traditional counterparts but with extended capability.
See *Table 18-2*

**chroot(2)** -> Change process root directory

**dirname(2)** -> Get path prefix of a path
See *Table 18-3*

**basename(2)** -> Get file name of a path
See *Table 18-3*

## Tables

- See **Table 18-1** for how various functions interprete symlinks.

- See **Table 18-2** for system calls that use a directory fd to interpret
relative pathnames.

- See **Table 18-3** for example strings returned by *dirname(2)* and *basename(2)*

## More Info

- See *t_unlink.c* for a program that demonstrates how unlinking a
  file with an open file discriptor does not deallocate its data
  blocks.

- For random access within a DIR see *telldir(3)* and *seekdir(3)*.

- For a demo of *nftw(3)* see *nftw_dir_tree.c*

- For a demo of *opendir()*, *readdir()*, and *closedir()* see
  *list_files.c*

## TODO
