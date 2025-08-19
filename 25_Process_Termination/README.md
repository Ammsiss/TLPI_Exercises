# Chapter 25: Process Termination
## Ex. 25-1

**Question**

If a child process makes the call exit(-1), what exit status will be seen by the parent?

**Answer**

See *negative_exit_status.c*

The exit status returned was seemingly random on my machine.

```bash
$ ./negative_exit_status.bin
Exit status: 18715
$ ./negative_exit_status.bin
Exit status: 18779
$ ./negative_exit_status.bin
Exit status: 18843
```
