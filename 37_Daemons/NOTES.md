## Syscalls

becomeDaemon

**syslog API**
    openlog
    syslog
    closelog
    setlogmask

## Tables

**Listing 37-1:** Flags to selectively inhibit certain steps of becoming a daemon.

**Figure 37-1**: Overview of system logging (syslog)
**Table 37-1:** facility values for openlog() and the priority argument of syslog()
**Table 37-2**: level values for the priority argument of syslog() (from highest to lowest severity)

## More Info

Perhaps the best source of further information about writing daemons is the
source code of various existing daemons.

## TODO
