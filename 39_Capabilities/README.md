# Chapter 39: Capabilities
## Ex. 39-1

**Question**

Modify the program in Listing 35-2 (sched_set.c, on page 743) to use file capabilities,
so that it can be used by an unprivileged user.

**Answer**

See *capable_sched_set.c*

In order to run you first have to add the CAP_SYS_NICE capability into the permitted
capability set. I did so with the following:

```bash
$ sudo setcap "cap_sys_nice=p" capable_sched_set.bin
```

Then an unprivileged process can add it to their effective capability set and sub-
sequently execute sched_setscheduler().
