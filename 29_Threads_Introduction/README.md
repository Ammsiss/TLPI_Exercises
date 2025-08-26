# Chapter 29: Exercises
## Ex. 29-1

**Question**

What possible outcomes might there be if a thread executes the following code:

```c
pthread_join(pthread_self(), NULL);
```

Write a program to see what actually happens on Linux. If we have a variable, *tid*,
containing a thread ID, how can a thread prevent itself from making a call,
*pthread_join(tid, NULL)*, that is equivalent to the above statement?

**Answer**

See *self_join.c*

When I attempted it *pthread_join()* failed with errno message:

```
pthread_join: Resource deadlock avoided
```

Which makes sense because if a thread is waiting for itself that could lead to
infinite blocking. To verify against a self join you can use the *pthread_self()*
call to verify the *tid* in question before calling *pthread_join()*

## Ex. 29-2

**Question**

Aside from the absence of error checking and various variable and structure
declarations, what is the problem with the following program?

```c
static void *threadFunc(void *arg)
{
    struct someStruct *pbuf = (struct someStruct *) arg;
    /* Do some work with structure pointed to by 'pbuf' */
}

int main(int argc, char *argv[])
{
    struct someStruct buf;
    pthread_create(&thr, NULL, threadFunc, (void *) &buf);
    pthread_exit(NULL);
}
```

**Answer**

The main function sends the memory address of buf to the start funciton
but then immediately calls *pthread_exit()* which causes its stack memory
(which includes buf) to be cleared while the other threads still continue
to execute. Now the second thread is working with undefined memory.
