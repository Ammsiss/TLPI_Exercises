# Chapter 36: Process Resources
## Ex. 36-1

**Question**

Write a program that shows that the getrusage() RUSAGE_CHILDREN flag retrieves
information about only the children for which a wait() call has been performed.
(Have the program create a child process that consumes some CPU time, and then
have the parent call getrusage() before and after calling wait().)

**Answer**

See *test_rusage_flag.c*

## Ex. 36-2

**Question**

Write a program that executes a command and then displays its resource usage.
This is analogous to what the time(1) command does. Thus, we would use this
program as follows:

```bash
$ ./rusage command arg...
```

**Answer**

See *display_usage.c*

## Ex. 36-3

**Question**

Write programs to determine what happens if a process’s consumption of various
resources already exceeds the soft limit specified in a call to setrlimit()

**Answer**

See *limit_passed_cap.c*

With the exception of **RLIMIT_NICE** (and of the ones I tested), the limits can
be set lower then the current resource value without anything blowing up.

Of course you still can't go higher unless your current consumption of the given
resource lowers to satisfy the new limit.
