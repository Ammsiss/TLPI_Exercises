## Syscalls

**sigqueue(3)** -> Sends a *realtime* signal

**sigsuspend(2)** -> Sets the signal mask and then blocks, Like *pause()*

**sigwaitinfo(2)** -> Block until a specified signal is delivered
**sigtimedwait(2)** -> Same as *sigwaitinfo()*, except only blocks for specified time
**signalfd(2)** -> Same as sigwaitinfo, except returns a file descriptor to the signal

### System V signal API funcitons (obsolete)

**sigset(3)** -> Establish a signal handler

**sighold(3)** -> Add a signal to the process signal mask
**sigrelse(3)** -> Remove a signal from the process signal mask
**sigignore(3)** -> Sets a signals disposition to **SIG_IGN**
**sigpause(3)** -> Like *sigsuspend()*, except can only unblock 1 signal

### BSD signal API (obsolete)

**sigvec(3)** -> Analogous to *sigaction()*

**sigblock(3)** -> Adds a set of signals to the process signal mask
**sigsetmask(3)** -> Sets an absolute value for the signal mask

**sigpause(3)** -> Analogous to *sigsuspend()*

**sigmask(3)** ->  Creates a mask from a signal number

## Tables

See **Table 22-1** for format specifiers for /proc/sys/kernel/core_pattern

## More Info

See **core(5)** for more details about core dump files.

See **sysv_signal(3)** for unreliable signal semantics with modern versions of glibc.

See errata 402

## TODO

[] Redo the inotify mechanism with the simplified buffer manaagement as shown in
signalfd_sigval.c

[] Make a local machine 2 process messaging program using only signals
