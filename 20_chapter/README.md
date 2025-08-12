# Chapter 20: Signals: Fundamental Concepts
## Ex. 20-1

**Question**

As noted in Section 20.3, sigaction() is more portable than signal() for establishing a
signal handler. Replace the use of signal() by sigaction() in the program in Listing 20-7
(sig_receiver.c).

**Answer**

See *use_sigaction.c*

## Ex. 20-2

**Question**

Write a program that shows that when the disposition of a pending signal is
changed to be SIG_IGN, the program never sees (catches) the signal.

**Answer**

See *clear_pending.c*

In the program I block all signals and then send the SIGUSR1 signal
to the program itself using *raise(3)* in order for it to set the bit in the
pending mask. To prove that the pending signal will be cleared from the mask I
set the disposition of SIGUSR1 to SIG_IGN and then re-print the pending signals
to check for the absense of the signal.

This output seems to confirm it.

```bash
Printing pending signals...
10 (User defined signal 1)

Setting SIGUSR1 to SIG_IGN...

Printing pending signals...
<empty signal set>
```

## Ex. 20-3

**Question**

Write programs that verify the effect of the SA_RESETHAND and SA_NODEFER flags when
establishing a signal handler with sigaction().

**Answer**

See *confirm_resethand.c* and *confirm_nodefer.c*

**Question**

Implement *siginterrupt()* using *sigaction()*

**Answer**

This solution is included in the Chapter 21 directory.
(See the page 419 errata @ man7.org/tlpi/errata/index.html)
