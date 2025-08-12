## Syscalls

**sigaction(2)** -> Set the disposition of a signal

**signal(2)** -> Don't use this! Shitty and not portable

**kill(2)** -> Send a signal from one process to another

**raise(3)** -> Send a signal to the calling process or thread

**killpg(3)** -> Sends a signal to all members of a process group

**strsignal(3)** -> Retrieve a signal description (See *sys_siglist[]*)

**psignal(3)** -> Prints message and signal description

**sigemptyset(3)** -> Initialize a signal set to contain no members
**sigfillset(3)** -> Initialize a signal set to contain all members

**sigaddset(3)** -> Add a signal to a set
**sigdelset(3)** -> Remove a signal from a set

**sigismember(3)** -> Tests a signal's membership of a set

**sigandset(3)** -> Places the intersection of 2 sets into a new set
**sigorset(3)** -> Places the union of 2 sets into a new set

**sigisemptyset(3)** -> Checks if a set has no signals

**sigprocmask(2)** -> Modify or retrieve processes signal mask

**sigpending(2)** -> Returns the set of pending signals

## Tables

See **Table 20-1** for various information about linux signals.

## More Info

See section **20.5** for the permission rules for interprocess
signal communication.

The sigaction structure is defined as something like:
```c
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
    void     (*sa_restorer)(void);
};
```

## TODO
