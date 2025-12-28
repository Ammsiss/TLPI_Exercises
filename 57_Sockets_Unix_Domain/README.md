# Chapter 57: Sockets: Unix Domain

## Ex. 57-1

**Question**

In **Section 57.3**, we noted that UNIX domain datagram sockets
are reliable. Write programs to show that if a sender transmits
datagrams to a UNIX domain datagram socket faster than the
receiver reads them, then the sender is eventually blocked, and
remains blocked until the receiver reads some of the pending
datagrams.

**Answer**

See *udds_reliable.c*

The sender puts 'r' onto stdout every time it successfully sends
a message and the reader puts 'w' when it reads one. The sender
attempts a send every 40th of a second whearas the receiver only
processes them at a tenth of a second.

At first the output looked like this...

```
srssssrssssrssssrssssrssssrsss...
```

...but eventually the sender was blocked.

```
...rssssrssrssrsssrsssrrrrrrrr...
```

Then when suffient messages were read, sending resumed.

```
...rrrrrrrrrrrrrrrrrrsrssssrss
```

## Ex. 57-2

**Question**

Rewrite the programs in **Listing 57-3** (*us_xfr_sv.c*) and
**Listing 57-4** (*us_xfr_cl.c*) to use the Linux-specific
abstract socket namespace (**Section 57.6**).

**Answer**

See *send_msg.h*, *send_msg_cl.c* and *send_msg_sv.c*

## Ex. 57-3

**Question**

Reimplement the sequence-number server and client of **Section
44.8** using UNIX domain stream sockets.

**Answer**

See *seqnum_us.h*, *seqnum_us_cl.c* and *seqnum_us_sv.c*

## Ex. 57-4

**Question**

Suppose that we create two UNIX domain datagram sockets bound to
the paths */somepath/a* and */somepath/b*, and that we connect
the socket */somepath/a* to */somepath/b*. What happens if we
create a third datagram socket and try to send (`sendto()`) a
datagram via that socket to */somepath/a*? Write a program to
determine the answer. If you have access to other UNIX systems,
test the program on those systems to see if the answer differs.

**Answer**

See *con_datagram_test.c*

When the third unrelated socket attempted to send to the
connected socket `sendto()` failed and set `errno` to `EPERM`.

This means that the restriction that "*Only datagrams sent
by the peer socket may be read on the socket.*" applies to the
sender not the receiver.
