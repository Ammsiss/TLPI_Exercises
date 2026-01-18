# Chapter 37: Daemons

## Ex. 37-1

**Question**

Write a program (similar to logger(1)) that uses syslog(3) to write arbitrary messages
to the system log file. As well as accepting a single command-line argument
containing the message to be logged, the program should permit an option to
specify the level of the message.

**Answer**

See *sys_logger.c*
