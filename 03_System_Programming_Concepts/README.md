# Chapter 3: Exercises

## Ex. 3-1

**Question**

When using the Linux-specific `reboot()` system call to reboot
the system, the second argument, magic2, must be specified as
one of a set of magic numbers (e.g., `LINUX_REBOOT_MAGIC2`).
What is the significance of these numbers? (Converting them to
hexadecimal provides a clue.)

**Answer**

The numbers form dates. For example 0x28121969 is the date
December 28th, 1969 which is also Linus Torvalds birthday. There
are 2 other constants that form dates and they are the birthdays
of Linus's two daughters.
