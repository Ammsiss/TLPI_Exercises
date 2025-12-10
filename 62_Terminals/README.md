# Chapter 62: Terminals
## Ex. 62-1

**Question**

Implement isatty(). (You may find it useful to read the description of tcgetattr() in
Section 62.2.)

**Answer**

The following should do it.

```c
/* Fails with ENOTTY if fd does not refer to a terminal */
int rv = tcgetattr(fd, dummy);
if (rv == -1) {
    return (errno == ENOTTY) ? false : -1;
} else
    return true;
```

## Ex. 62-2

**Question**

Implement ttyname().

**Answer**

See *ttyname_imp.c*

This one came with an interesting snag! Because there are 2 (or more) potential
spots for terminal files to live, namely /dev and /dev/pts, you have to be careful
not to just compare inode numbers. On my machinie /dev and /dev/pts are both separate
mount points so when I initially wrote the program I was getting some seemingly random
terminal path of /dev/null, when my real terminal was /dev/pts/1. This was because both
of those terminals shared the smae Inode number which is allowed because they were on
diferent file systems. (I confirmed this with the following simple script: ./find_file-
_with_inode.sh).

After also comparing against the device number stored in the stat object the problem was
solved!

## Ex. 62-3

**Question**

Implement the getpass() function described in Section 8.5. (The getpass() function
can obtain a file descriptor for the controlling terminal by opening /dev/tty.)

**Answer**

See *getpass_imp.c*

## Ex. 62-4

**Question**

Write a program that displays information indicating whether the terminal
referred to by standard input is in canonical or noncanonical mode. If in
noncanonical mode, then display the values of TIME and MIN.

**Answer**

See *canon_test.c*
