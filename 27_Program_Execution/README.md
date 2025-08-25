# Chapter 27: Program Execution
## Ex. 27-1

**Question**

The final command in the following shell session uses the program in Listing 27-3
to exec the program xyz. What happens?

```bash
$ echo $PATH
/usr/local/bin:/usr/bin:/bin:./dir1:./dir2
$ ls -l dir1
total 8
-rw-r--r-- 1 mtk users 7860 Jun 13 11:55 xyz
$ ls -l dir2
total 28
-rwxr-xr-x 1 mtk users 27452 Jun 13 11:55 xyz
$ ./t_execlp xyz
```

**Answer**

The 'p' series of *exec()* functions search the PATH environment variable from left
to right, therefore it will first see that a program named 'xyz' exists within *./dir1*
and try to execute it. It will fail do so as the file does not have execute permissions
so it will then search *./dir2*, find 'xyz' and execute it.

## Ex. 27-2

**Question**

Use *execve()* to implement *execlp()*. You will need to use the *stdarg(3)* API to handle
the variable-length argument list supplied to *execlp()*. You will also need to use
functions in the *malloc* package to allocate space for the argument and environment
vectors. Finally, note that an easy way of checking whether a file exists in a particular
directory and is executable is simply to try execing the file.

**Answer**

See *imp_execlp.c*

## Ex. 27-3

**Question**

What output would we see if we make the following script executable and exec() it?

```bash
#!/bin/cat -n
Hello world
```

**Answer**

When we call *exec()* the script file provides the script shebang provides the interpretor
path and optional arg, and the program provides the script-path and arguments. In this case
cat is being invoked something like:

```bash
cat -n <script_name> [extra]
```

**script_name** is the name of the script that containts the shebang and the **extra** opts
are the ones provided by the exec() function (Excluding *argv\[0\]*). Therefore cat simply
ptints the contents of the script file (with line numbers).

## Ex. 27-4

**Question**

What is the effect of the following code? In what circumstances might it be useful?

```c
childPid = fork();
if (childPid == -1)
    errExit("fork1");

if (childPid == 0) {   /* Child */
    switch (fork()) {
    case -1:
        errExit("fork2");
    case 0: /* Grandchild */
        /* ----- Do real work here ----- */
        exit(EXIT_SUCCESS); /* After doing real work */
    default:
        exit(EXIT_SUCCESS); /* Make grandchild an orphan */
    }
}

/* Parent falls through to here */

if (waitpid(childPid, &status, 0) == -1)
    errExit("waitpid");

/* Parent carries on to do other things */
```

**Answer**

The nice thing about this is that the parent does not need to worry about waiting for
the grandchild to exit (eg. setting up a handler). The parent simply needs to wait for
the direct child to exit (which is quick and synchronous) as the grand child will
be orphaned leading to an automatic reaping by init.

## Ex. 27-5

**Question**

When we run the following program, we find it produces no output. Why is this?

```c
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    printf("Hello world");
    execlp("sleep", "sleep", "0", (char *) NULL);
}
```

**Answer**

*printf()* is usually line buffered so "Hello world" is just sitting in the libc buffer.
Then when *execlp()* is called the *entire* process image is replaced including the libc
buffers. If you want the output to be printed you can call *fflush()* to explicilty flush
the libc buffer befor calling *exec()*.

## Ex. 27-3

**Question**

Suppose that a parent process has established a handler for **SIGCHLD** and also
blocked this signal. Subsequently, one of its children exits, and the parent then
does a *wait()* to collect the child’s status. What happens when the parent unblocks
SIGCHLD? Write a program to verify your answer. What is the relevance of the result
for a program calling the *system()* function?

**Answer**

See *double_handle.c*

When the parent unblocks **SIGCHLD** it is immediately delivered. This can cause
issues if the handler, as it often is, is set up to also attempt to wait for the
child process. For example if the handler uses *wait()* and there are other live
children then *wait()* will block until a one of them changes status. (This can
be handled without blocking using *waitpid()* with the **WNOHANG** flag)

*system()* also blocks **SIGCHLD** therefore the setup is the same.
