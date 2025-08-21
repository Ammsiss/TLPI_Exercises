# Chapter 26: Monitoring Child Processes
## Ex. 26-1

**Question**

Write a program to verify that when a child’s parent terminates, a call to getppid()
returns 1 (the process ID of init).

**Answer**

See *adopted_child.c*

**Question**

Suppose that we have three processes related as grandparent, parent, and child,
and that the grandparent doesn’t immediately perform a wait() after the parent
exits, so that the parent becomes a zombie. When do you expect the grandchild to
be adopted by init (so that getppid() in the grandchild returns 1): after the parent
terminates or after the grandparent does a wait()? Write a program to verify your
answer.

**Answer**

See *adoption_when.c*

The program creates a parent->child->grandchild relationship. The middle child immediately
exits and then 'grandchild' prints its ppid. The program uses a signal to wait until
'grandchild' prints its ppid before 'parent' calls wait. Then 'grandchild' prints its ppid
again to compare.

So from the output it seems as though the grandchild is adopted by init immediately
rather then after the grand parent reaps the middle child.

```bash
Parent waiting for signal. (PID=120505)
Child started. PID=120506 PPID=120505
Child exiting...
Grand child started. PID=120507 PPID=1 <- (Immediately adopted...)
Parent calling wait()...
Grand child: PID=120507 PPID=1
Grand child leaving...
Parent exiting...
```

**Question**

Replace the use of waitpid() with waitid() in Listing 26-3 (child_status.c). The call to
printWaitStatus() will need to be replaced by code that prints relevant fields from
the siginfo_t structure returned by waitid().

**Answer**

See *t_waitid.c*

**Question**

Listing 26-4 (make_zombie.c) uses a call to sleep() to allow the child process a chance
to execute and terminate before the parent executes system(). This approach
produces a theoretical race condition. Modify the program to eliminate the race
condition by using signals to synchronize the parent and child.

**Answer**

See *tsafe_zombie.c*

Instead of signals I found the WNOWAIT flag with the *waitid()* call to be a clean solution.
