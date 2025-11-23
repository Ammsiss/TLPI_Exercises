# Chapter 34: Process Groups, Sessions, and Job Control
## Ex. 34-1

**Question**

Suppose a parent process performs the following steps:

```c
/* Call fork() to create a number of child processes, each of which
remains in same process group as the parent */
/* Sometime later... */
signal(SIGUSR1, SIG_IGN); /* Parent makes itself immune to SIGUSR1 */
killpg(getpgrp(), SIGUSR1); /* Send signal to children created earlier */
```

What problem might be encountered with this application design? (Consider shell
pipelines.) How could this problem be avoided?

**Answer**

## Ex. 34-2

**Question**

Write a program to verify that a parent process can change the process group ID of
one of its children before the child performs an exec(), but not afterward.

**Answer**

See *change_after_exec.c*

## Ex. 34-3

**Question**

Write a program to verify that a call to setsid() from a process group leader fails.

**Answer**

See *proc_leader_setsid.c*

## Ex. 34-4

**Question**

Modify the program in Listing 34-4 (disc_SIGHUP.c) to verify that, if the controlling
process doesn’t terminate as a consequence of receiving SIGHUP, then the kernel
doesn’t send SIGHUP to the members of the foreground process.

**Answer**

See *cproc_sighup_fail.c*

I simply added a handler for SIGHUP in the parent process and ran the same command
(exec ./bin d s s > sig.log 2>&1) to demonstrate that after the parent caught SIGHUP
(and did not terminate as a consequence) the processes in the foreground group
did not recieve SIGHUP.

## Ex. 34-5

**Question**

Suppose that, in the signal handler of Listing 34-6, the code that unblocks the
SIGTSTP signal was moved to the start of the handler. What potential race condition
does this create?

**Answer**

```c
static void /* Handler for SIGTSTP */
tstpHandler(int sig)
{
    sigset_t tstpMask, prevMask;
    int savedErrno;
    struct sigaction sa;

    savedErrno = errno; /* In case we change 'errno' here */

    /* Unblock SIGTSTP; the pending SIGTSTP immediately suspends the program */

    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
        errExit("sigprocmask");

    printf("Caught SIGTSTP\n"); /* UNSAFE (see Section 21.1.2) */

    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        errExit("signal"); /* Set handling to default */

    raise(SIGTSTP); /* Generate a further SIGTSTP */


    /* Execution resumes here after SIGCONT */
    if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
        errExit("sigprocmask"); /* Reblock SIGTSTP */

    sigemptyset(&sa.sa_mask); /* Reestablish handler */
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpHandler;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Exiting SIGTSTP handler\n");
    errno = savedErrno;
}
```

## Ex. 34-6

**Question

Write a program to verify that when a process in an orphaned process group
attempts to read() from the controlling terminal, the read() fails with the error EIO

**Answer**

See *orphan_read.c*

Simply executing the following works as the parent would be the only process in the
process group to have a parent (the shell) in the same session but a different process group.

```c
if (fork() != 0)
    exit(EXIT_SUCCESS);
```

then attempting a read in the child resulted in a EIO error.

Note: You must redirect the output of the program in order to see the error message as
stdout by default is hooked up to the controlling terminal and of course that would not
work as the child is in an orphaned process group and can't read or write to the controlling
terminal as demonstrated.

## Ex. 34-7

**Question**

Write a program to verify that if one of the signals SIGTTIN, SIGTTOU, or SIGTSTP is sent
to a member of an orphaned process group, then the signal is discarded (i.e., has
no effect) if it would stop the process (i.e., the disposition is SIG_DFL), but is
delivered if a handler is installed for the signal.

**Answer**

See *job_sig_to_orphan.c*

This programs takes an argument whether or not to set up a custom handler in the orphaned
process group child for SIGTSTP. The child then sleeps and waits for a signal. If you
do set up a signal handler the signal is caught and the handler is envoked. If you do
not then nothing happens. The process is not stopped. You must redirect the output to a
file.
