## Syscalls

dlopen -> loads a shared library
dlclose -> unload a shared library

dlerror -> basically dlopen api version of errno string

dlsym -> Manually load symbol (can return NULL in both error and non error cases, so
        use dlerror)
dladdr -> Get info about where a address came from

## Tables
## More Info

In order to use
a version script, we must specify the ––version–script linker option:

```bash
$ gcc -Wl,--version-script,myscriptfile.map ...
```

To build programs that use the dlopen API on Linux, we must specify the –ldl
option, in order to link against the libdl library

```c
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    funcp = (void (*)(void)) dlsym(libHandle, argv[2]);
#pragma GCC diagnostic pop

    /* In the book, instead of the preceding line, the code uses a
       rather clumsy looking cast of the form:

           *(void **) (&funcp) = dlsym(libHandle, argv[2]);

       This was done because the ISO C standard does not require compilers
       to allow casting of pointers to functions back and forth to 'void *'.
       (See TLPI pages 863-864.) SUSv3 TC1 and SUSv4 accepted the ISO C
       requirement and proposed the clumsy cast as the workaround. However,
       the 2013 Technical Corrigendum to SUSv4 requires implementations
       to support casts of the more natural form (now) used in the code
       above. However, various current compilers (e.g., gcc with the
       '-pedantic' flag) may still complain about such casts. Therefore,
       we use a gcc pragma to disable the warning.

       Note that this pragma is available only since gcc 4.6, released in
       2010. If you are using an older compiler, the pragma will generate
       an error. In that case, simply edit this program to remove the
       lines above that begin with '#pragma".

       See also the erratum note for page 864 at
       http://www.man7.org/tlpi/errata/. */
```
- For further details on the topic of symbol visibility, see Drepper, 2004 (b).

- Example version script file for only allowing some symbols to be visible
```c
VER_1 {
    global:
        vis_f1;
        vis_f2;
    local:
        *;
};
```

- If we assign the value help to LD_DEBUG, the dynamic linker displays help informa-
tion about LD_DEBUG, and the specified command is not executed:

```bash
$ LD_DEBUG=help date
```

## TODO

- Look deeper into symbol versioning
