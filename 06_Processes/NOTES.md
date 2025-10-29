## Syscalls

**getpid(2)** -> Get the process ID of the calling process
**getppid(2)** -> Get the parent process ID of the calling process

**getenv(3)** -> Get the value of a named environment variable
**putenv(3)** -> Add a environment variable
**setenv(3)** -> More flexible version of putenv()
**putenv(3)** -> Removes an environment variable
**clearenv(3)** -> Clears the environment

**setjmp(3)** -> Set a long jump point
**longjmp(3)** -> Jump to a long jump point

## Tables

**Figure 6-1**: Typical memory layout of a process on Linux/x86-32

**Figure 6-2**: Overview of virtual memory

**Figure 6-3**: Example of a process stack

**Figure 6-4**: Values of argc and argv for the command necho hello world

**Figure 6-5**: Example of process environment list data structures

## More Info

*Tanenbaum, 2007* and *Vahalia, 1996* describe virtual memory management in
detail. The Linux kernel memory management algorithms and code are described
in detail in *Gorman, 2004*.

## TODO
