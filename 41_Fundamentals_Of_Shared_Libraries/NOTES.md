## Syscalls

Not syscalls but very useful commands:

ldd -> List dynamic dependencies

objdump and readelf -> Both display's various libary related information

nm -> list symbols defined in an object library or executable program

ldconfig -> Automates shared library symlink name creations
    (The command ldconfig –p displays the current contents of /etc/ld.so.cache)

## Tables

**Figure 41-1** shows the compilation and linking steps involved in producing a shared
library with an embedded soname, linking a program against that shared library,
and creating the soname symbolic link needed to run the program.

**Figure 41-2** shows the steps that occur when the program created in Figure 41-1 is
loaded into memory in preparation for execution.

**Table 41-1**: Summary of shared library names

**Figure 41-3**: Conventional arrangement of shared library names

**Figure 41-4**: A shared library that depends on another shared library

**Figure 41-5**: Resolving a global symbol reference

## More Info

For finding Shared Libraries at Run Time see 41.11

In order to determine whether an existing object file has been compiled with
the –fPIC option, we can check for the presence of the name _GLOBAL_OFFSET_TABLE_
in the object file’s symbol table, using either of the following commands:

```bash
$ nm mod1.o | grep _GLOBAL_OFFSET_TABLE_
$ readelf -s mod1.o | grep _GLOBAL_OFFSET_TABLE_
```

Conversely, if either of the following equivalent commands yields any output, then
the specified shared library includes at least one object module that was not com-
piled with –fPIC:

```bash
$ objdump --all-headers libfoo.so | grep TEXTREL
$ readelf -d libfoo.so | grep TEXTREL
```

The string TEXTREL indicates the presence of an object module whose text segment
contains a reference that requires run-time relocation

The dynamic linker is just the process of resolving the embedded library name
at run time.

We can use the following commands to examine prog and libx1.so in order to
see the contents of their rpath lists:

```bash
$ objdump -p prog | grep PATH
RPATH /home/mtk/pdir/d1 libx1.so will be sought here at run time
$ objdump -p d1/libx1.so | grep PATH
RPATH /home/mtk/pdir/d2 libx2.so will be sought here at run time
```

- Various information related to static and shared libraries can be found in the ar(1),
gcc(1), ld(1), ldconfig(8), ld.so(8), dlopen(3), and objdump(1) manual pages and in the
info documentation for ld and readelf. Drepper, 2004 (b) covers many of the finer
details of writing shared libraries on Linux. Further useful information can also be
found in David Wheeler’s Program Library HOWTO, which is online at the LDP web
site, http://www.tldp.org/. The GNU shared library scheme has many similarities to
that implemented in Solaris, and therefore it is worth reading Sun’s Linker and
Libraries Guide (available at http://docs.sun.com/) for further information and
examples. Levine, 2000 provides an introduction to the operation of static and
dynamic linkers.

- Information about GNU Libtool, a tool that shields the programmer from the
implementation-specific details of building shared libraries, can be found online at
http://www.gnu.org/software/libtool and in Vaughan et al., 2000.
The document Executable and Linking Format, from the Tools Interface
Standards committee, provides details on ELF. This document can be found
online at http://refspecs.freestandards.org/elf/elf.pdf. Lu, 1995 also provides a lot of
useful detail on ELF.

## TODO

- As a good exersize try and run a system with no package manager (This will force
    you to get quite familiar with libararies, ldconfig, and appreciate what the
    package manager actually does)

- Make a toy "turn-key" application (see page 854)
