# Chapter 49: Memory Mappings
## Ex. 49-1

**Question**

Write a program, analogous to cp(1), that uses mmap() and memcpy() calls (instead
of read() or write()) to copy a source file to a destination file. (Use fstat() to obtain
the size of the input file, which can then be used to size the required memory
mappings, and use ftruncate() to set the size of the output file.)

**Answer**

See *mmap_cp.c*

## Ex. 49-2

**Question**

Rewrite the programs in Listing 48-2 (svshm_xfr_writer.c, page 1003) and Listing 48-3
(svshm_xfr_reader.c, page 1005) to use a shared memory mapping instead of System V
shared memory

**Answer**

Not doing System V related questions.

## Ex. 49-3

**Question**

Write programs to verify that the SIGBUS and SIGSEGV signals are delivered in the
circumstances described in Section 49.4.3.

**Answer**

See *boundary_case.c*

## Ex. 49-4

**Question**

Write a program that uses the MAP_FIXED technique described in Section 49.10 to
create a nonlinear mapping similar to that shown in Figure 49-5.

**Answer**
