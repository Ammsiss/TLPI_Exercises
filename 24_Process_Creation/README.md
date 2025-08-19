# Chapter 24: Process Creation
## Ex. 24-1

**Question**

After a program executes the following series of fork() calls, how many new
processes will result (assuming that none of the calls fails)?

```c
fork();
fork();
fork();
```

**Answer**

7 additional processes. The total process count doubles with each call to fork. 2 after
the first, 4 after the second and 8 after the last.

## Ex. 24-2

**Question**

Write a program to demonstrate that after a vfork(), the child process can close a
file descriptor (e.g., descriptor 0) without affecting the corresponding file descriptor
in the parent.

**Answer**

See *vfork_close.c*

## Ex. 24-3

**Question**

Assuming that we can modify the program source code, how could we get a core
dump of a process at a given moment in time, while letting the process continue
execution?

**Answer**

You could create a child process that immediately calls raise() with a signal that
produces a core dump file.

## Ex. 24-4

**Question**

Experiment with the program in Listing 24-5 (fork_whos_on_first.c) on other UNIX
implementations to determine how these implementations schedule the parent
and child processes after a fork().

**Answer**

## Ex. 24-5

**Question**

Suppose that in the program in Listing 24-6, the child process also needed to wait
on the parent to complete some actions. What changes to the program would be
required in order to enforce this?

**Answer**

See *double_wait.c*
