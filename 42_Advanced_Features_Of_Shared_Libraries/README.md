# Chapter 42: Advanced Features of Shared Libraries
## Ex. 42-1

**Question**

Write a program to verify that if a library is closed with dlclose(), it is not unloaded if
any of its symbols are used by another library.

**Answer**

See *dlclose_busy.c*

It seems as though no matter what if the reference count hits 0, the dlopen API does consider
the handle "closed" but its still loaded and accessible from other libraries that still need
the symbols. I wonder if there is a way to verify if when none of the symbols are being
used by another library if it is unloaded after dlopen considers it "closed".

## Ex. 42-2

**Question**

Add a dladdr() call to the program in Listing 42-1 (dynload.c) in order to retrieve
information about the address returned by dlsym(). Print out the values of the fields
of the returned Dl_info structure, and verify that they are as expected.

**Answer**

The output looks about right.

```bash
$ LD_LIBRARY_PATH=. ./dynload_dladdr.bin libreturn.so return_symbol
Path name of shared library: ./libreturn.so
Base address of shared library: 140308526616576
Name of nearest runtime symbol: return_symbol
Acutal name of the symbol returned in dli_sname: 140308526617481
```
