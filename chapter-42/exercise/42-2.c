#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    Dl_info info;
    void *lib_handler;
    int (*funcp)(const char *, ...);
    const char *err;

    lib_handler = dlopen(argv[1], RTLD_LAZY);
    if (lib_handler == NULL)
        fatal("dlopen: %s", dlerror());

    (void) dlerror();
    *(void **)(&funcp) = dlsym(lib_handler, argv[2]);
//    printf("DD\n");
    err = dlerror();
    if (err != NULL)
        fatal("dlsym: %s", err);

    if (funcp == NULL)
        printf("%s is NULL\n", argv[2]);
    else
    {
        (*funcp)("fuck\n");
//        printf("fuck\n");
        dladdr(*(void **)(&funcp), &info);
        printf("pathname:%s\n"
               "baseaddr:%p\n"  
               "symname :%s\n"
               "symaddr :%p\n",
               info.dli_fname, info.dli_fbase,
               info.dli_sname, info.dli_saddr);
    }
    dlclose(lib_handler);
    exit(EXIT_SUCCESS);
}