## Syscalls

**wait(2)** -> Wait for a child proccess to terminate
**waitpid(2)** -> Wait for a specific child to terminate

**waitid(2)** -> A more featured *waitpid()*

**wait3(2)** -> Return resource usage of any child
**wait4(2)** -> Return resource usage of a specific child

## Tables
## More Info

## Idioms

```c
// Wait for all children to termiante
while ((childPid = wait(NULL)) != -1)
    continue;
if (errno != ECHILD)   /* An unexpected error... */
    errExit("wait");

// Typically body of SIGCHILD handler
while (waitpid(-1, NULL, WNOHANG) > 0)
    continue;
```


## TODO

Make a program that fills the kernels process table with zombie entries (might
crash pc)
