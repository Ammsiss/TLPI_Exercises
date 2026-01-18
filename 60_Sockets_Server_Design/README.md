# Chapter 60: Sockets: Server Design

## Ex. 60-1

**Question**

Add code to the program in **Listing 60-4** (*is_echo_sv.c*) to
place a limit on the number of simultaneously executing
children.

**Answer**

See *inetd_echo_sv.c*.

## Ex. 60-2

**Question**

Sometimes, it may be necessary to write a socket server so that
it can be invoked either directly from the command line or
indirectly via inetd. In this case, a command- line option is
used to distinguish the two cases. Modify the program in
**Listing 60-4** so that, if it is given a `–i` command-line
option, it assumes that it is being invoked by `inetd` and
handles a single client on the connected socket, which inetd
supplies via `STDIN_FILENO`. If the `–i` option is not supplied,
then the program can assume it is being invoked from the command
line, and operate in the usual fashion. (This change requires
only the addition of a few lines of code.) Modify
*/etc/inetd.conf* to invoke this program for the echo service.

**Answer**

See *inetd_echo_sv.c*.
