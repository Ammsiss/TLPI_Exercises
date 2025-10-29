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

## Ex. 18-2

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

## Ex. 18-3

**Question**

Implement *realpath(3)*

**Answer**

See *imp_realpath.c*

## Ex. 18-4

**Question**

Modify the program in Listing 18-2 (list_files.c) to use readdir_r()
instead of readdir().

**Answer**

See *ls_readdir_r.c*

## Ex. 18-5

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

I ran into a tricky issue with this problem. Initially I compared the inode
stored in the dirent structure with the inode stored in the stat structure
but this does not work. The inode in the dirent structure never reflects
mount-points on the directory, so it will disagree with the inode reported
by **stat(2)**, which *does* follow mounts.

Heres an example of the problem that occurred with the /home mount point.
```cpp

/* This compares the original inode number of "home" with the root inode
   of the /home file-system. Not the same!

   On my filesystem (Btrfs) the default root inode was 256. but the original
   directory inode was 259. */

if (entry->d_ino == current_sb.st_ino) { // mismatch at a mount point

struct stat entry_sb;
if (stat(entry->d_name, &entry_sb) == -1)
    errExit("stat");

    if (entry_sb.st_dev == current_sb.st_dev) {
        printf("%s\n", entry->d_name);
        break;
    }
}
```

To fix this I simply called stat on every entry and compared the 2 stat entries
as shown in the implementation.

## Ex. 18-6

**Question**

Modify the program in Listing 18-3 (nftw_dir_tree.c) to use the FTW_DEPTH flag. Note
the difference in the order in which the directory tree is traversed.

**Answer**

The program already has the functionality to use the FTW_DEPTH flag. Simply pass in
'd' as a command line argument.

This causes a post order traversal meaning that the contents of a directory are proc-
essed before the directory itself.

Note that this does not enforce any ordering of siblings. For example if you have a file
and a subdirectory in the same directory level, the file may be processed before the sub-
directories children.

## Ex. 18-7

**Question**

Write a program that uses nftw() to traverse a directory tree and finishes by
printing out counts and percentages of the various types (regular, directory,
symbolic link, and so on) of files in the tree.

**Answer**

See *tree_info.c*

## Ex. 18-8

**Question**

Implement nftw(). (This will require the use of the opendir(), readdir(), closedir(), and
stat() system calls, among others.)

**Answer**

See *imp_nftw.c*

## Ex. 18-9

**Question**

In Section 18.10, we showed two different techniques (using fchdir() and chdir(),
respectively) to return to the previous current working directory after changing the
current working directory to another location. Suppose we are performing such an
operation repeatedly. Which method do you expect to be more efficient? Why?
Write a program to confirm your answer.

**Answer**

See *chdir_vs_fchdir.c* for the test program.

I will assume that the version calling getcwd will be more preformant as there
is probably a more significant cost to repeatedly allocating and freeing a file
discriptor as it invokes kernal level processes.

My test seems to agree as the system time is elevated quite a bit.

```bash
Method 1 (Using buffer):
System: 0.2100   User: 0.0100
Method 2 (Using fd):
System: 0.3200   User: 0.0200
```

This assumes that the path name resolution is not very expensive though. For example
I ran the same test program nested within 100 directories with the following results.

```bash
Method 1 (Using buffer):
System: 4.8900   User: 0.0100
Method 2 (Using fd):
System: 0.3400   User: 0.0300
```

fchdir is also the safer option as the original directory can be renamed or moved and
fchdir will still succeed.
