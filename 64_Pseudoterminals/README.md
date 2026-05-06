# Chapter 64 : Pseudoterminals
## Ex. 64-1

**Question**

In what order do the script parent process and the child shell
process terminate when the user types the end-of-file character
(usually Control-D) while running the program in **Listing
64-3**? Why?

**Answer**

The child shell process terminates first. This is because the
parent process was set to raw mode causing the C-D character to
be interpreted literally and passed to the master. The master
then sends it to the slave which is in canonical mode which
causes the childs next read to return EOF and subsequently close
the last fd for the slave and exit. Then the parent attempts a
read and sees EIO and subsequently exits.

## Ex. 64-2

**Question**

Make the following modifications to the program in **Listing
64-3** (*script.c*):

1. The standard `script(1)` program adds lines to the beginning
   and the end of the output file showing the time the script
   started and finished. Add this feature.
2. Add code to handle changes to the terminal window size as
   described in **Section 64.7**. You may find the program in
   **Listing 62-5** (*demo_SIGWINCH.c*) useful for testing this
   feature.

**Answer**

See *better_script.c*

## Ex. 64-3

**Question**

Modify the program in **Listing 64-3** (*script.c*) to replace
the use of `select()` by a pair of processes: one to handle data
transfer from the terminal to the pseudoterminal master, and the
other to handle data transfer in the opposite direction.

**Answer**

See *fork_script.c*

## Ex. 64-4

**Question**

Modify the program in **Listing 64-3** (*script.c*) to add a
time-stamped recording feature. Each time the program writes a
string to the typescript file, it should also write a
time-stamped string to a second file (say, *typescript.timed*).
Records written to this second file might have the following
general form:

`<timestamp> <space> <string> <newline>`

The timestamp should be recorded in text form as the number of
milliseconds since the start of the script session. Recording
the timestamp in text form has the advantage that the resulting
file is human-readable. Within string, real newline characters
will need to be escaped. One possibility is to record a newline
as the 2-character sequence `\n` and a backslash as `\\`.

Write a second program, *script_replay.c*, that reads the
time-stamped script file and displays its contents on standard
output at the same rate at which they were originally written.
Together, these two programs provide a simple recording and
playback feature for shell session logs.

**Answer**

See *better_script.c* and *replay_script.c*

## Ex. 64-5

**Question**

Implement client and server programs to provide a simple
telnet-style remote login facility. Design the server to handle
clients concurrently (**Section 60.1**). **Figure 64-3** shows
the setup that needs to be established for each client login.
What isn’t shown in that diagram is the parent server process,
which handles incoming socket connections from clients and
creates a server child to handle each connection. Note that all
of the work of authenticating the user and starting a login
shell can be dealt with in each server child by having the
(grand)child created by `ptyFork()` go on to `exec login(1)`.

**Answer**

See *telnet_sv.c*, *telnet_cl.c*, *telnet.h*

## Ex. 64-6

**Question**

Add code to the program developed in the previous exercise to
update the login accounting files at the start and end of the
login session (**Chapter 40**).

**Answer**

On my operating system this program doesn't really work so
instead, as an extra exersize, I added another "out of band"
socket to each client connection so that window size updates on
the clients terminal would be reflected in the slave.

## Ex. 64-7

**Question**

Suppose we execute a long-running program that slowly generates
output that is redirected to a file or pipe, as in this example:

```bash
$ longrunner | grep str

```
One problem with the above scenario is that, by default, the
stdio package flushes standard output only when the stdio buffer
is filled. This means that the output from the longrunner
program will appear in bursts separated by long intervals of
time. One way to circumvent this problem is to write a program
that does the following:

1. Create a pseudoterminal.
2. Exec the program named in its command-line arguments with the
   standard file descriptors connected to the pseudoterminal
   slave.
3. Read output from the pseudoterminal master and write it
   immediately to standard output (`STDOUT_FILENO`, file
   descriptor 1), and, at the same time, read input from the
   terminal and write it to the pseudoterminal master, so that
   it can be read by the execed program.

Such a program, which we’ll call unbuffer, would be used as
follows:

```bash
$ ./unbuffer longrunner | grep str
```

Write the unbuffer program. (Much of the code for this program
will be similar to that of **Listing 64-3**.)

**Answer**

See *unbuffer.c* and *longrunner.c*

## Ex. 64-8

**Question**

Write a program that implements a scripting language that can be
used to drive `vi` in a noninteractive mode. Since `vi` expects
to be run from a terminal, the program will need to employ a
pseudoterminal.

**Answer**

See *drivi.c*
