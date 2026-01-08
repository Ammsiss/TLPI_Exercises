# Chapter 43: IPC Overview
## Ex. 43-1

**Question**

Write a program that measures the bandwidth provided by pipes. As command-
line arguments, the program should accept the number of data blocks to be sent
and the size of each data block. After creating a pipe, the program splits into two
process: a child that writes the data blocks to the pipe as fast as possible, and a
parent that reads the data blocks. After all data has been read, the parent should
print the elapsed time required and the bandwidth (bytes transferred per second).
Measure the bandwidth for different data block sizes.

**Answer**

## Ex. 43-2

**Question**

Repeat the preceding exercise for System V message queues, POSIX message
queues, UNIX domain stream sockets, and UNIX domain datagram sockets. Use
these programs to compare the relative performance of the various IPC facilities
on Linux. If you have access to other UNIX implementations, perform the same
comparisons on those systems.

**Answer**
