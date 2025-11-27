#define _GNU_SOURCE
#include <dlfcn.h>

#include "../lib/error_functions.h"

int main(int argc, char *argv[])
{
    void *libHandle;
    void (*funcp)(void);
    const char *err;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s lib-path func-name\n", argv[0]);

    libHandle = dlopen(argv[1], RTLD_LAZY);
    if (libHandle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    (void) dlerror();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    funcp = (void (*)(void)) dlsym(libHandle, argv[2]);
#pragma GCC diagnostic pop

    err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "dlsym: %s\n", err);
        exit(EXIT_FAILURE);
    }

    Dl_info info;   /* Get info about this memory address */
    if (dladdr(funcp, &info) == 0)
        errExit("dladdr: (No Message)");

    printf("Path name of shared library: %s\n", info.dli_fname);
    printf("Base address of shared library: %ld\n", (long) info.dli_fbase);
    printf("Name of nearest runtime symbol: %s\n", info.dli_sname);
    printf("Acutal name of the symbol returned in dli_sname: %ld\n", (long) info.dli_saddr);

    (*funcp)();

    dlclose(libHandle);

    exit(EXIT_SUCCESS);
}
