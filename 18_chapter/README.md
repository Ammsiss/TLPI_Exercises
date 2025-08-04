# Chapter 18: Directories and links
## Ex. 18-1

**Question**

In Section 4.3.2, we noted that it is not possible to open a file for writing if it is
currently being executed (open() returns –1, with errno set to ETXTBSY).

Nevertheless, it is possible to do the following from the shell:

```bash
$ cc -o longrunner longrunner.c
$ ./longrunner & Leave running in background
$ vi longrunner.c Make some changes to the source code
$ cc -o longrunner longrunner.c
```

The last command overwrites the existing executable of the same name. How is this
possible? (For a clue, use ls –li to look at the i-node number of the executable file
after each compilation.)

**Answer**

When you recompile the program it simply removes and replaces the hard
link stored in the directory, hence why the inode number changes.
The original executable itself is never modified.

**Question**

Why does the call to chmod() in the following code fail?

```c
mkdir("test", S_IRUSR | S_IWUSR | S_IXUSR);
chdir("test");
fd = open("myfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
symlink("myfile", "../mylink");
chmod("../mylink", S_IRUSR);
```

**Answer**

'mylink' is a dangling link. The call to symlink() resolves the
relative path, 'myfile', from where the link is stored not where
symlink() is executed thus the subsequent call to chmod fails with
**ENOENT**. (chmod does not operator on sym links)

**Question**

Modify the program in Listing 18-2 (list_files.c) to use readdir_r()
instead of readdir().

**Answer**

See *ls_readdir_r.c*

**Question**

Implement a function that performs the equivalent of getcwd().

A useful tip for solving this problem is that you can find the name of the
current working directory by using opendir() and readdir() to walk through
each of the entries in the parent directory (..) to find an entry with the
same i-node and device number as the current working directory (i.e.,
respectively, the st_ino and st_dev fields in the stat structure returned
by stat() and lstat()). Thus, it is possible to construct the directory
path by walking up the directory tree (chdir(“..”)) one step at a time and
performing such scans.

The walk can be finished when the parent directory is the same as the current
working directory (recall that /.. is the same as /). The caller should be
left in the same directory in which it started, regardless of whether your
getcwd() function succeeds or fails (open() plus fchdir() are handy for this
purpose).

**Answer**

See *imp_getcwd.c*

**Question**

Modify the program in Listing 18-3 (nftw_dir_tree.c) to use the FTW_DEPTH flag. Note
the difference in the order in which the directory tree is traversed.

**Answer**



**Question**

Write a program that uses nftw() to traverse a directory tree and finishes by
printing out counts and percentages of the various types (regular, directory,
symbolic link, and so on) of files in the tree.

**Answer**



**Question**

Implement nftw(). (This will require the use of the opendir(), readdir(), closedir(), and
stat() system calls, among others.)

**Answer**



**Question**

In Section 18.10, we showed two different techniques (using fchdir() and chdir(),
respectively) to return to the previous current working directory after changing the
current working directory to another location. Suppose we are performing such an
operation repeatedly. Which method do you expect to be more efficient? Why?
Write a program to confirm your answer.

**Answer**
