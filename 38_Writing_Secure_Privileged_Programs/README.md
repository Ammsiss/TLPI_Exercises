# Chapter 38: Writing Secure Privileged Programs
## Ex. 38-1

**Question**

Log in as a normal, unprivileged user, create an executable file (or copy an existing
file such as /bin/sleep), and enable the set-user-ID permission bit on that file (chmod
u+s). Try modifying the file (e.g., cat >> file). What happens to the file permissions
as a result (ls –l)?

Why does this happen?

**Answer**

The change made could have been malicious and if it was a set uid root program that could
have some pretty bad consequences.

## Ex. 38-2

**Question**

Write a set-user-ID-root program similar to the sudo(8) program. This program
should take command-line options and arguments as follows:

```bash
$ ./douser [-u user ] program-file arg1 arg2 ...
```

The douser program executes program-file, with the given arguments, as though it
was run by user. (If the –u option is omitted, then user should default to root.) Before
executing program-file, douser should request the password for user, authenticate it
against the standard password file (see Listing 8-2, on page 164), and then set all of
the process user and group IDs to the correct values for that user.

**Answer**

See *./douser.c*

**Beware, this program is horrifyingly unsafe as a globally executable setuid-root program.**
