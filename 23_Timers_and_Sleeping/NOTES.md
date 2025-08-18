## Syscalls

**getitimer(2)** -> Retrieve an *interval timer*
**setitimer(2)** -> Establish an *interval timer*

**alarm(2)** -> Set a simple timer

**sleep(2)** -> Suspend execution for a given amount of time
**nanosleep(2)** -> Higher resolution *sleep()* among other advantages

### POSIX Clocks API

**clock_gettime(2)** -> Return the time of a clock specified by *clockid*
**clock_getres(2)** -> Returns the resolution of a clock specified by *clockid*

**clock_settime(2)** -> Sets a clock specified in by *clockid*

**clock_getcpuclockid(2)** -> Retrive the *clockid* of a process
**pthread_getcpuclockid(2)** -> Retrieve the *clockid* if a process or thread

**clock_nanosleep(2)** -> Similar to *nanosleep()* but you can specify a clock

### POSIX Interval Timers

**timer_create(2)** -> Creates a new timer

**timer_settime(2)** -> Arm (start) or disarm (stop) a timer

**timer_gettime(2)** -> Returns interval and remaining time of a timer

**timer_delete(2)** -> Free the resources used by a POSIX timer

**timer_getoverrun(2)** -> Return the overrun count of a timer

**timerfd_create(2)** -> Return fd for a new timer
**timerfd_settime(2)** -> Return fd for a new timer
**timerfd_gettime(2)** -> Return fd for a new timer

## Tables

See **Table 23-2** for values for the *sigev_notify* field of the *sigevent* structure.

## More Info

Ensure that you use only one of *setitimer()* and *alarm()*.

For max portability avoid mixing the use of *sleep()*, *alarm()*, and *setitimer()*

You must link against -lrt (librt, realtime library) to use the POSIX timer API

## TODO

See the solutions in the appendix of the TLPI book for exersise 23-2 and 23-3.

After reading the chapters on threads return to section 23.6.7
