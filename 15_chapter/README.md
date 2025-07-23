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
    circuits the moment it finds a match. That is if the effective uid of the process
    matches the uid of the file then the permissions are granted based on the owner
    permissions of the file even if they are less permissive then the group or other
    permissions.

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


