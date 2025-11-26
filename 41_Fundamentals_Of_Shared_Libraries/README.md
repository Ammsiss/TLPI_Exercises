# Chapter 41: Fundamentals of Shared Libraries
## Ex. 41-1

**Question**

Try compiling a program with and without the –static option, to see the difference
in size between an executable dynamically linked with the C library and one that is
linked against the static version of the C library.

**Answer**

First I had to install the static version of glibc:

```bash
$ sudo dnf install glibc-static
```
The size of the executable statically linked was 3.2 MB, while the dynamically
linked one clocked in at a measly 13 KB.
