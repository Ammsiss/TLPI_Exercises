# Chapter 55: File Locking
## Ex. 55-1

**Question**

Experiment by running multiple instances of the program in Listing 55-1
(t_flock.c) to determine the following points about the operation of flock():

a) Can a series of processes acquiring shared locks on a file starve a process
attempting to place an exclusive lock on the file?

b) Suppose that a file is locked exclusively, and other processes are waiting to
place both shared and exclusive locks on the file. When the first lock is
released, are there any rules determining which process is next granted a
lock? For example, do shared locks have priority over exclusive locks or
vice versa? Are locks granted in FIFO order?

c) If you have access to some other UNIX implementation that provides
flock(), try to determine the rules on that implementation.

**Answer**

## Ex. 55-2

**Question**

Write a program to determine whether flock() detects deadlock situations when
being used to lock two different files in two processes.

**Answer**

See *flock_dead.c* (It doesn't)

## Ex. 55-3

**Question**

Write a program to verify the statements made in Section 55.2.1 regarding the
semantics of inheritance and release of flock() locks.

**Answer**

## Ex. 55-4

**Question**

Experiment by running the programs in Listing 55-1 (t_flock.c) and Listing 55-2
(i_fcntl_locking.c) to see whether locks granted by flock() and fcntl() have any effect
on one another. If you have access to other UNIX implementations, try the same
experiment on those implementations.

**Answer**

## Ex. 55-5

**Question**

In Section 55.3.4, we noted that, on Linux, the time required to add or check for
the existence of a lock is a function of the position of the lock in the list of
all locks on the file.

Write two programs to verify this:

a) The first program should acquire (say) 40,001 write locks on a file. These
locks are placed on alternating bytes of the file; that is, locks are placed on
bytes 0, 2, 4, 6, and so on through to (say) byte 80,000. Having acquired
these locks, the process then goes to sleep.

b) While the first program is sleeping, the second program loops (say)
10,000 times, using F_SETLK to try to lock one of the bytes locked by the
previous program (these lock attempts always fail). In any particular execu-
tion, the program always tries to lock byte N * 2 of the file.

Using the shell built-in time command, measure the time required by the second
program for N equals 0, 10,000, 20,000, 30,000, and 40,000. Do the results match
the expected linear behavior?

**Answer**

See *create_locks.c* and *flock_pref_t.c*

My output after every 10'000 lock increase roughly increased linearly.

```
$ time ./flock_pref_t.bin 0
./flock_pref_t.bin 0  0.00s user 0.01s system 97% cpu 0.011 total
$ time ./flock_pref_t.bin 10000
./flock_pref_t.bin 10000  0.00s user 0.85s system 99% cpu 0.858 total
$ time ./flock_pref_t.bin 20000
./flock_pref_t.bin 20000  0.00s user 1.70s system 99% cpu 1.713 total
$ time ./flock_pref_t.bin 30000
./flock_pref_t.bin 30000  0.00s user 2.56s system 99% cpu 2.578 total
$ time ./flock_pref_t.bin 40000
./flock_pref_t.bin 40000  0.00s user 3.37s system 99% cpu 3.395 total
```

0.011 -> 0.858 (0.847 sec increase)
0.858 -> 1.713 (0.855 sec increase)
1.713 -> 2.578 (0.865 sec increase)
2.578 -> 3.395 (0.817 sec increase)

## Ex. 55-6

**Question**

Experiment with the program in Listing 55-2 (i_fcntl_locking.c) to verify the
statements made in Section 55.3.6 regarding lock starvation and priority for fcntl()
record locks.

**Answer**

To see my verification of lock starvation on linux see *lock_starve.c* and for testing
of the priority of record lock requests see below.

I created 1 process (55758) that acquired a full file write lock and then created 5
more processes that each requested a read lock request with F_SETLKW.

The output below shows the order of the read lock requests by job number (The process
corresponding to job 2 sent the first request and the process corresponding to job 6 sent
the last request).

I then had the first process unlock its write lock and observed the order in which the
5 read lock requests were processed.

```
$ jobs -l
[1]  + 55758 suspended  ./i_fcntl_locking.bin test_file.txt
[2]    55841 running    ./i_fcntl_locking.bin test_file.txt
[3]    55914 running    ./i_fcntl_locking.bin test_file.txt
[4]    55980 running    ./i_fcntl_locking.bin test_file.txt
[5]    56047 running    ./i_fcntl_locking.bin test_file.txt
[6]  - 56178 running    ./i_fcntl_locking.bin test_file.txt
$ fg %1
[1]    continued  ./i_fcntl_locking.bin test_file.txt
PID=55758> s u 0 0
[PID=55758] unlocked
[PID=55914] got lock
[PID=55980] got lock
[PID=55841] got lock
[PID=56047] got lock
[PID=56178] got lock
```

**ORDER**: Job2->Third, Job3->First, Job4->Second, Job5->Fourth, Job6->Fifth

## Ex. 55-7

**Question**

If you have access to other UNIX implementations, use the program in Listing 55-2
(i_fcntl_locking.c) to see if you can establish any rules for fcntl() record locking
regarding starvation of writers and regarding the order in which multiple queued
lock requests are granted.

**Answer**

## Ex. 55-8

**Question**

Use the program in Listing 55-2 (i_fcntl_locking.c) to demonstrate that the kernel
detects circular deadlocks involving three (or more) processes locking the same file.

**Answer**

After setting it up so that A -> B -> C, I attempted to create a lock with F_SETLKW in
C. Then after resuming each process, C did in fact get the deadlock signal as shown in
the output below.

(Job 1 -> A, Job 2 -> B, Job 3 -> C)

```
[1]    continued  ./i_fcntl_locking.bin test_file.txt
[2]  - continued  ./i_fcntl_locking.bin test_file.txt
[3]  - continued  ./i_fcntl_locking.bin test_file.txt
[PID=176025] failed (deadlock)
```

## Ex. 55-9

**Question**

Write a pair of programs (or a single program that uses a child process) to bring
about the deadlock situation with mandatory locks described in Section 55.4.

**Answer**

## Ex. 55-10

**Question**

Read the manual page of the lockfile(1) utility that is supplied with procmail. Write a
simple version of this program.

**Answer**

See *lockfile_imp.c*

This program just uses the older locking mechanism of using open with O_CREAT | O_EXCL.
The output below shows what happens with the program if you run it specifying 10 lock
files of which 6-10 already exist, sleeping for 1 second if a lockfile creation failes
and retrying a maximum of 3 seconds.

```
$ ./lockfile_imp.bin 1 3 test{1..10}.txt
Created lockfile test1.txt
Created lockfile test2.txt
Created lockfile test3.txt
Created lockfile test4.txt
Created lockfile test5.txt
Attempt #1 - test6.txt: Failed; Waiting 1 sec
Attempt #2 - test6.txt: Failed; Waiting 1 sec
Attempt #3 - test6.txt: Failed; Waiting 1 sec
ERROR: Failed to create lockfile test6.txt
```
