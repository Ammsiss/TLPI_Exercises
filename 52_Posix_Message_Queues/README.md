# Chapter 52: Posix Message Qeueues

## Ex. 52-1

**Question**

Modify the program in **Listing 52-5** (*pmsg_receive.c*) to
accept a timeout (a relative number of seconds) on the comand
line, and use `mq_timedreceive()` instead of `mq_receive()`.

**Answer**

See *timed_receive.c*

## Ex. 52-2

**Question**

Recode the sequence-number client-server application of
**Section 44.8** to use POSIX message queues.

**Answer**

See *pmsg_file.h*, *pmsg_seqnum_client.c*, and
*pmsg_seqnum_server.c*

## Ex. 52-3

**Question**

Recode the file-server application of **Section 46.8** to use
POSIX message queues instead of System V message queues.

**Answer**

See *pmsg_file.h*, *pmsg_file_client.c*, and
*pmsg_file_server.c*

## Ex. 52-4

**Question**

Write a simple chat program (Similar to `talk(1)`, but without
the curses interface) using POSIX message queues.

**Answer**

See *pmsg_chat.c*

I based my program off the `write(2)` command instead as it is
essentially `talk()` without the curses interface.

## Ex. 52-5

**Question**

Modify the progarm in **Listing 52-6** (*mq_notify_sig.c*) to
demonstrate that message queue notification established by
`mq_notify()` occurs just once. This can be done by removing the
`mq_notify()` call within the for loop.

**Answer**

See *notify_once.c*

## Ex. 52-6

**Question**

Replace the use of a signal handler in **Listing 52-6**
(*mq_notify_sig.c*) with the use of `sigwaitinfo()`. Upon return
of `sigwaitinfo()`, display the values returned in the
*siginfo_t* structure. How could the program obtain the message
queue discriptor in the *siginfo_t* structure returned by
`sigwaitinfo()`?

**Answer**

See *sigwaitinfo_notify.c*

You can get the message queue discriptor by adding it to the
`sigev_value.sival_int` field in the *sigevent* structure
during registration and then accessing it from the
`si_value.sival_int` field of the *siginfo_t* structure.

## Ex. 52-7

**Question**

In **Listing 52-7** (*mq_notify_thread.c*), could `buffer` be
made a global variable and its memory allocated just once (in
the main program)? Explain your answer.

**Answer**

Only if you wait on the child thread before re-registering for
another notification. If you re-register immediately then
another thread would be created and you would have 2 threads
that could access the same shared memory location (threads share
heap memory).
