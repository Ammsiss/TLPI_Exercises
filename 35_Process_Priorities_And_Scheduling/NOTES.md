## Syscalls

getpriority
setpriority

## Tables

**Figure 35-1**: Range and interpretation of the process nice value

## More Info

Love, 2010 provides background detail on process priorities and scheduling on
Linux. Gallmeister, 1995 provides further information about the POSIX realtime
scheduling API. Although targeted at POSIX threads, much of the discussion of
the realtime scheduling API in Butenhof, 1996 is useful background to the real-
time scheduling discussion in this chapter.

For further information about CPU affinity and controlling the allocation of
threads to CPUs and memory nodes on multiprocessor systems, see the kernel
source file Documentation/cpusets.txt, and the mbind(2), set_mempolicy(2), and cpuset(7)
manual pages.

## TODO

Look further into the realtime stuff and do the exersizes.
