# Chapter 15: File Attributes
## Ex. 15-1

**Question**

Section 15.4 contained several statements about the permissions required for
various file-system operations. Use shell commands or write programs to verify or
answer the following:

a) Removing all owner permissions from a file denies the file owner access,
even though group and other do have access.

b) On a directory with read permission but not execute permission, the
names of files in the directory can be listed, but the files themselves can’t
be accessed, regardless of the permissions on them.

c) What permissions are required on the parent directory and the file itself in
order to create a new file, open a file for reading, open a file for writing,
and delete a file? What permissions are required on the source and target
directory to rename a file? If the target file of a rename operation already
exists, what permissions are required on that file? How does setting the
sticky permission bit (chmod +t) of a directory affect renaming and deletion
operations?

**Answer**

a) This is true as the algorithm that the kernal uses to verify file access short
circuits the moment it finds a match.

That is if the effective uid of the process matches the uid of the file then the
permissions are granted based on the owner permissions of the file even if they are
less permissive then the group or other permissions.

For a shell script verifying this behaviour see *owner_access_denied.sh*
For a c program verifying this behaviour see *owner_access_denied.c*

b) For a shell script veryifing this behaviour see *read_only_dir.sh*
For a c program veryifing this behaviour see *read_only_dir.c*

c) In order to create a new file you must have write and execute permissions on the
parent directory.

In order to open a file for reading you only need execute permissions on the parent
directory and read permissions on the file itself. (You must know the name of the file)

In order to open a file for writing you only need execute permissions on the parent
directory and write permissions on the file itself. (You must know the name of
the file)

In order to delete a file you need write and execute permissions on the parent directory
and any permissions on the file itself.

In order to rename a file you are essentailly doing a deletion in the source and creation
in the target. This requires read and execute in both directories.

If the target of a rename operation already exists you must have write perms on the file
to overwrite it.

If the sticky bit is set you cannot overwrite the file unless you own it or the parent
directory.

For scripts verifying some of these assertions see *perm_tests.sh*

## Ex. 15-2

**Question**

Do you expect any of a file’s three timestamps to be changed by the stat() system
call? If not, explain why.

**Answer**

stat(1) probably shouldn't update any of the file timestamps considering it only
views meta data about the file.

Access time requires file *content* access, modfication time requires file *content*
modfication and status change time only referes to the *modification* of the meta
data.

## Ex. 15-3

**Question**

On a system running Linux 2.6, modify the program in Listing 15-1 (t_stat.c) so
that the file timestamps are displayed with nanosecond accuracy.

**Answer**

See *t_stat_nano.c*

## Ex. 15-4

**Question**

The access() system call checks permissions using the process’s real user and group
IDs. Write a corresponding function that performs the checks according to the
process’s effective user and group IDs.

**Answer**

See *effective_access.c*


## Ex. 15-5

**Question**

As noted in Section 15.4.6, umask() always sets the process umask and, at the same
time, returns a copy of the old umask. How can we obtain a copy of the current
process umask while leaving it unchanged?

**Answer**

You could assing the return value of umask() to a variable and then call umask again
with the value of that variable. For example:

```c
mode_t umaskValue = umask(0); // Get and clear umask
umask(umaskValue);            // Reassign old umask
```

## Ex. 15-6

**Question**

The chmod a+rX file command enables read permission for all categories of user,
and likewise enables execute permission for all categories of user if file is a directory
or execute permission is enabled for any of the user categories for file, as
demonstrated in the following example:

```bash
$ ls -ld dir file prog
dr-------- 2 mtk users 48 May 4 12:28 dir
-r-------- 1 mtk users 19794 May 4 12:22 file
-r-x------ 1 mtk users 19336 May 4 12:21 prog
$ chmod a+rX dir file prog
$ ls -ld dir file prog
dr-xr-xr-x 2 mtk users 48 May 4 12:28 dir
-r--r--r-- 1 mtk users 19794 May 4 12:22 file
-r-xr-xr-x 1 mtk users 19336 May 4 12:21 prog

```

**Answer**

See *arX_mod.c*

## Ex. 15-7

**Question**

Write a simple version of the chattr(1) command, which modifies file i-node flags.
See the chattr(1) man page for details of the chattr command-line interface. (You
don’t need to implement the –R, –V, and –v options.)

**Answer**

See *imp_chattr.c*
