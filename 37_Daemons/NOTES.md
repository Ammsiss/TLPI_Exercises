## Syscalls

`openlog()`
`syslog()`
`closelog()`
`setlogmask()`

## Listing

**Listing 37-1**: (*become_daemon.h*) Header file for
*become_daemon.c*

**Listing 37-2**: (*become_daemon.c*) Creating a daemon process

## Tables

**Figure 37-1**: Overview of system logging (syslog)

**Table 37-1:** facility values for openlog() and the priority
argument of syslog()

**Table 37-2**: level values for the priority argument of
syslog() (from highest to lowest severity)

## More Info

**Daemonization steps:**

First the parent process calls `fork()` and then closes which
cuases the daemon to be inherited by init. This is necessary as
it means the daemon will not be a process group leader as it
would have a different pid then its inherited pgid. This allows
step 1 to succeed as a process can only call `setsid()` if it is
*not* a process group leader.

1. call `setsid()` to lose assocaition with controlling
   terminal. This means that no job control signals will be
   generated for example the shell propogating `SIGHUP` to all
   background process groups.
2. call `fork()` again so that the daemon is not a session
   leader so that it cannot acquire a new controlling terminal.
3. clear the process umask to get requested permissions on file
   creation.
4. set the current working directory to */* so that at shutdown
   the file system will not block an unmount.
5. close file descriptors 0, 1, and 2 (which would refer to the
   previous terminal device) and open */dev/null* on them

Perhaps the best source of further information about writing daemons is the
source code of various existing daemons.

## TODO
