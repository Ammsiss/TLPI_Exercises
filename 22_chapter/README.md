# Chapter 22: Signals: Advanced Features
## Ex. 22-1

**Question**

Section 22.2 noted that if a stopped process that has established a handler for and
blocked SIGCONT is later resumed as a consequence of receiving a SIGCONT, then the
handler is invoked only when SIGCONT is unblocked. Write a program to verify this.
Recall that a process can be stopped by typing the terminal suspend character
(usually Control-Z) and can be sent a SIGCONT signal using the command kill –CONT
(or implicitly, using the shell fg command).

**Answer**

See *verify_sigcont.c*

## Ex. 22-2

**Question**

If both a realtime and a standard signal are pending for a process, SUSv3 leaves it
unspecified which is delivered first. Write a program that shows what Linux does in
this case. (Have the program set up a handler for all signals, block signals for a
period time so that you can send various signals to it, and then unblock all signals.)

**Answer**

No matter the orientation of signals sent while the program was blocking the standard
signals seem to be delivered first.

See *signal_race.c*

## Ex. 22-3

**Question**

Section 22.10 stated that accepting signals using sigwaitinfo() is faster than the use
of a signal handler plus sigsuspend(). The program signals/sig_speed_sigsuspend.c,
supplied in the source code distribution for this book, uses sigsuspend() to
alternately send signals back and forward between a parent and a child process.
Time the operation of this program to exchange one million signals between the
two processes. (The number of signals to exchange is provided as a command-line
argument to the program.) Create a modified version of the program that instead
uses sigwaitinfo(), and time that version. What is the speed difference between the
two programs?

**Answer**

See *sig_speed_sigwaitinfo.c*

There was an approximitaly 3.5 second difference.

```bash
$ time ./sig_speed_sigsuspend.bin 1000000
./sig_speed_sigsuspend.bin 1000000  1.01s user 5.46s system 47% cpu 13.528 total
$ time ./sig_speed_sigwaitinfo.bin 1000000
r./sig_speed_sigwaitinfo.bin 1000000  0.71s user 4.03s system 47% cpu 10.037 total
```

## Ex. 22-4

**Question**

Implement the System V functions sigset(), sighold(), sigrelse(), sigignore(), and
sigpause() using the POSIX signal API.

**Answer**

See *system_v_funcs.c*
