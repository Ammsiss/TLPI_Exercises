# Chapter 53: POSIX Semaphores

## Ex. 53-1

**Question**

Rewrite the programs in **Listing 48-2** (*svshm_xfr_writer.c*)
and **Listing 48-3** (*svshm_xfr_reader.c*) as a threaded
application, with the two threads passing data to each other via
a global buffer, and using POSIX semaphores for synchronization.

**Answer**

See *sem_data_transfer.c*

## Ex. 53-2

**Question**

Modify the program in **Listing 53-3** (*psem_wait.c*) to use
`sem_timedwait()` instead of `sem_wait().` The program should
take an additional command-line argument that specifies a
(relative) number of seconds to be used as the timeout for the
`sem_timedwait()` call.

**Answer**

See *psem_timedwait.c*

## Ex. 53-3

**Question**

Devise an implementation of POSIX semaphores using System V
semaphores.

**Answer**

Not touching System V.

## Ex. 53-4

**Question**

In **Section 53.5**, we noted that POSIX semaphores perform much
better than System V semaphores in the case where the semaphore
is uncontended. Write two programs (one for each semaphore type)
to verify this. Each program should simply increment and
decrement a semaphore a specified number of times. Compare the
times required for the two programs.

**Answer**

See *psem_perf_test.c*

Pretty fast! 100 million iterations in 0.649 seconds.

```bash
$ time ./psem_perf_test.bin 10000000
```
```
./psem_perf_test.bin 10000000  0.07s user 0.00s system 99% cpu 0.068 total
```
```bash
$ time ./psem_perf_test.bin 100000000
```
```
./psem_perf_test.bin 100000000  0.65s user 0.00s system 99% cpu 0.649 total
```
```bash
$ time ./psem_perf_test.bin 1000000000
```
```
./psem_perf_test.bin 1000000000  6.32s user 0.00s system 99% cpu 6.348 total
```

(*Didn't make the System V program*)
