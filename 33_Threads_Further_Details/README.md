# Chapter 33: Threads: Further Details
## Ex. 33-1

**Question**

Write a program to demonstrate that different threads in the same process can have
different sets of pending signals, as returned by sigpending(). You can do this by using
pthread_kill() to send different signals to two different threads that have blocked
these signals, and then have each of the threads call sigpending() and display
information about pending signals. (You may find the functions in Listing 20-4
useful.)

**Answer**

See *thread_signals.c*

## Ex. 33-2

**Question**

Suppose that a thread creates a child using fork(). When the child terminates, is it
guaranteed that the resulting SIGCHLD signal will be delivered to the thread that
called fork() (as opposed to some other thread in the process)?

**Answer**

The signal will be sent to an arbitrary thread that isn't blocking that signal. There
is no guarantee of which one.
