# Chapter 23: Timers and Sleeping
## Ex. 23-1

**Question**

Although alarm() is implemented as a system call within the Linux kernel, this is
redundant. Implement alarm() using setitimer()*

**Answer**

See *imp_alarm.c*

## Ex. 23-2

**Question**

Try running the program in Listing 23-3 (t_nanosleep.c) in the background with a
60-second sleep interval, while using the following command to send as many
SIGINT signals as possible to the background process:

```bash
$ while true; do kill -INT pid; done
```

You should observe that the program sleeps rather longer than expected. Replace
the use of nanosleep() with the use of clock_gettime() (use a CLOCK_REALTIME clock) and
clock_nanosleep() with the TIMER_ABSTIME flag. (This exercise requires Linux 2.6.)
Repeat the test with the modified program and explain the difference.

**Answer**

See *abstime_sleep.c*

When using *nanosleep()* in a loop to continue sleeping after an interupt we have to
take into account the rounding errors that *nanosleep()* incurs. If the program gets many
signals very fast the rounding errors compound.

The use of the TIMER_ABSTIME flag allows us to specify an absolute time with a clock,
removing this rounding issue as the requested time is static.

**Question**

## Ex. 23-3

**Question**

Write a program to show that if the evp argument to timer_create() is specified as
NULL, then this is equivalent to specifying evp as a pointer to a sigevent structure with
sigev_notify set to SIGEV_SIGNAL, sigev_signo set to SIGALRM, and si_value.sival_int set to
the timer ID

**Awnser**

See *evp_null.c*

## Ex. 23-4

**Question**

Modify the program in Listing 23-5 (ptmr_sigev_signal.c) to use sigwaitinfo() instead
of a signal handler.

**Awnser**

See *sequential.c*
