/*
    Note* When loading a library that is linked against another, the linker
    will try and automatically load all dependencies by default. So if you
    have libraries that are dependent on others and are all meant to be
    loaded dynamically (so they are in non-standard directories) then you
    should be sure to load them in the proper order as if you try to load
    one that is dependent on another first you will get an error.

    Note** Even if a depedent library is linked against its dependency
    AND that dependency is dynamically loaded, the linker still needs to be
    able to find the dependency automatically through the normal search paths.

    NOTE*** Also potentially the case (not confirmed) the string path you pass
    to dlopen is saved as is by the library loader as the naem of the library.
    So, if you have a library that is dependent on another both in non-standard
    directories and you try to load them in the right order with dlopen via relative
    paths they will be stored as you typed them. SO the linker will literally think
    that the library name is ./lib for example. This causes an issue if you then try
    to laod the dependent because if you linked against the dependency in any other
    way besides exactly what you passed to dlopen, it will fail saying it can't find
    the library.

    Takeaway: Just put dependencies in a place that the linker can find it if it needs
    too.

    Run this program with:
        LD_LIBRARY_PATH=. ./program
*/

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lib/error_functions.h"

int main(void)
{
                        /* libvalue.so has no dependencies */
    void *lib1_handle = dlopen("./libvalue.so", RTLD_LAZY);
    if (lib1_handle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    (void) dlerror();   /* Clear error string */
    int *value = (int *) dlsym(lib1_handle, "x");
    char *err = dlerror();         /* x is the name of the symbol in libvalue.so */
    if (err != NULL) {   /* Check if dlsym failed */
        fprintf(stderr, "dlsym: %s\n", err);
        exit(EXIT_FAILURE);
    }
                     /* libreturn.so uses symbols from libvalue.so */
    void *lib2_handle = dlopen("./libreturn.so", RTLD_LAZY);
    if (lib2_handle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    (void) dlerror();
    int (*funcp)(void) = (int (*)(void)) dlsym(lib2_handle, "return_symbol");
    err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "dlsym: %s\n", err);
        exit(EXIT_FAILURE);
    }

    /* This prints 5 so libreturn.so is currently using the symbol in libvalue.so... */
    printf("%d\n", (*funcp)());

    /* ...so lets try closing libvalue.so... */
    if (dlclose(lib1_handle) != 0) {
        fprintf(stderr, "dlclose: %s\n", dlerror());
    }

    /* ...and using its symbol again */
    printf("%d\n", (*funcp)());

    /* This triggers with 'dlclose: ./libvalue.so: shared object not open' so it is
     * closed just not unloaded yet as we can still access the symbol. */
    if (dlclose(lib1_handle) != 0) {
        fprintf(stderr, "dlclose: %s\n", dlerror());
    }

    if (dlclose(lib2_handle) != 0) {
        fprintf(stderr, "dlclose: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
