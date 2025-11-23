## Syscalls

getpgrp
setpgid

getsid
setsid

ctermid

tcgetpgrp
tcsetpgrp

## Tables

**Figure 34-1**: Relationships between process groups, sessions, and the controlling terminal
**Figure 34-2**: Job-control states
**Figure 34-3**: Steps in the creation of an orphaned process group

## More Info

Chapter 9 of *Stevens & Rago, 2005* covers similar material to this chapter, and
includes a description of the steps that occur during login to establish the session
for a login shell. The glibc manual contains a lengthy description of the functions
relating to job control and the implementation of job control within the shell. The
SUSv3 rationale contains an extensive discussion of sessions, process groups, and
job control.

## TODO

FINISH ALL THE EXERSIZES
