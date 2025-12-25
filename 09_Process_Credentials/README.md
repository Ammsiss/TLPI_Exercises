# Chapter 9: Exercises

## Ex. 9.1

**Question**

Assume in each of the following cases that the initial set of
process user IDs is real=1000 effective=0 saved=0 file-system=0.
What would be the state of the user IDs after the following
calls?

**Answer**

The procces is a set-UID-root program being executed by a user
with UID 1000.

a) `setuid(2000)`;

For unprivileged processes `setuid()` only effects the
effective-UID and it can only be changed to that of the
processes saved-set-UID or real-UID.

For prviliged procceses `setuid()` changes the real, effective and
saved-set UID with the file-system id mirroring the
effective-UID.

Therefore this call will result in the following values:

real=2000    effective=2000    saved=2000    file-system=2000.

b) `setreuid(–1, 2000)`;

For unprivileged processes `setreuid()` can only change the
effective-UID to the saved-set-UID or the real-UID.

For privilieged processes `setreuid()` can make arbitrary changes
to both ID's.

For both types of processes the saved-set-UID will not mirror
the effective-UID if both ruid is specified as -1 and the
effective-UID is specfified as the same value as the ruid prior
to the call.

Therefore this call will result in the following values:

real=1000    effective=2000    saved=2000    file-system=2000

c) `seteuid(2000)`;

For an unprivileged process `setuid()` and `seteuid()` perform
identically.

For prvilieged proccesses `seteuid()` can make arbitrary changes
to the effective-UID.

Therefore this call will result in the following values:

real=1000    effective=2000     saved=0    file-system=2000

d) `setfsuid(2000)`;

For an unprivilieged process `setfsuid()` can only change the
file-system-UID to either the effective-UID, saved-set-UID, or
real-UID.

For a prviliged process `setfsuid()` can change the file-system-UID
to any value.

Therefore this call will result in the following values:

real=1000    effective=0    saved=0    file-system=2000

e) `setresuid(–1, 2000, 3000)`;

For an unpriviliged process `setfsuid()` can only change each of
the values to each of the values prior to the call.

For privilieged processes `setfsuid()` can change each of the id's
to arbitrary values. (file-system-UID always mirrors the
        effective)

Therefore this call will result in the following values:

real=1000    effective=2000    saved=3000    file-system=2000

## Ex. 9.2

**Question**

Is a process with the following user IDs privileged? Explain
your answer.

real=0 effective=1000 saved=1000 file-system=1000

**Answer**

A processes privilege is determined solely by the
file-system-UID (and in practice the effective-UID) therefore
this process would not be privileged.

This is likely an example of a program that is owned by a non
root user with the set-UID bit enabled, being run by a root
user.

Though this program could regain priviliges through a call to
`seteuid()`

## Ex. 9.3

**Question**

Implement `initgroups()` using `setgroups()` and library
functions for retrieving information from the password and group
files (**Section 8.4**). Remember that a process must be
privileged in order to be able to call `setgroups()`.

**Answer**

See *initgroups_imp.c*

## Ex. 9.4

**Question**

If a process whose user IDs all have the value X executes a
set-user-ID program whose user ID, Y, is nonzero, then the
process credentials are set as follows:

real=X effective=Y saved=Y

(We ignore the file-system user ID, since it tracks the
effective user ID.) Show the `setuid()`, `seteuid()`, `setreuid()`,
and `setresuid()` calls, respectively, that would be used to
perform the following operations:

a) Suspend and resume the set-user-ID identity (i.e., switch the
   effective user ID to the value of the real user ID and then
   back to the saved set-user-ID).

b) Permanently drop the set-user-ID identity (i.e., ensure that
   the effective user ID and the saved set-user-ID are set to
   the value of the real user ID).

(This exercise also requires the use of `getuid()` and `geteuid()`
to retrieve the process’s real and effective user IDs.) Note
that for certain of the system calls listed above, some of
these operations can’t be performed.

**Answer**

`setuid()` & `seteuid()`

```c
uid_t euid = getuid(); // save effective-UID
setuid(getuid());      // set effective-UID to real-UID
setuid(euid);          // set effective-UID to saved-UID
```

`setreuid()`

```c
uid_t euid = getuid();  // save effective-UID
setreuid(-1, getuid()); // set effective-UID to real-UID
setreuid(-1, euid);     // set effective-UID to saved-UID
```

`setresuid()`

```c
uid_t euid = getuid();       // save effective-UID
setresuid(-1, getuid(), -1); // set effective-UID to real-UID
setresuid(-1, euid, -1);     // set effective-UID to saved-UID
```

`setreuid()`

```c
// set effective-UID & saved-UID to real-UID
setreuid(getuid(), getuid());
```

This works becuase of the following rule:

For both privileged and unprivileged processes, the saved
set-user-ID is also set to the same value as the (new) effective
user ID if either of the following is true:

a) ruid is not –1 (i.e., the real user ID is being set, even to
   the same value it already had), or

b) the effective user ID is being set to a value other than the
   value of the real user ID prior to the call.

Put conversely, if a process uses `setreuid()` only to change the
effective user ID to the same value as the current real user ID,
then the saved set-user-ID is left unchanged, and a later call
to `setreuid()` (or `seteuid()`) can restore the effective user ID
to the saved set-user-ID value. (SUSv3 doesn’t specify the
effect of `setreuid()` and `setregid()` on the saved set IDs, but
SUSv4 specifies the behavior described here.)

`setresuid()`

```c
// set all id's to real-UID
setresuid(getuid(), getuid(), getuid());
```

## Ex. 9.5

**Question**

Repeat the previous exercise for a process executing a
set-user-ID-root program, which has the following initial set of
process credentials:

real=X effective=0 saved=0

**Answer**

`setuid()`

cannot be used for this purpose.

`seteuid()`

```c
seteuid(getuid()); // set effective-UID to real-UID
seteuid(0);        // set effective-UID to saved-UID
```

`setreuid()`

```c
setreuid(-1, getuid()); // set effective-UID to real-UID
setreuid(-1, 0);        // set effective-UID to saved-UID
```

`setresuid()`

```c
setresuid(-1, getuid(), -1); // set effective-UID to real-UID
setresuid(-1, getuid(), -1); // set effective-UID to saved-UID
```

`setuid()`

```c
setuid(getuid()); // change all id's to real-UID
```

`seteuid()`

cannot be used for this purpose

`setreuid()`

```c
setreuid(getuid(), getuid());
```

`setresuid()`

```c
setresuid(getuid(), getuid(), getuid());
```
