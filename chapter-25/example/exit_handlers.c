#define _BSD_SOURCE
#include <stdlib.h>
#include "tlpi_hdr.h"

static void atexit_func1(void)
{
    printf("atexit function 1 called\n");
}

static void atexit_func2(void)
{
    printf("atexit function 2 called\n");
}

static void onexit_func(int exit_status, void *arg)
{
    printf("on_exit function called: status=%d, arg = %ld\n",
           exit_status, (long)arg);
}

int main(int argc, char *argv[])
{
    if (on_exit(onexit_func, (void *)10) != 0)
    {
        fatal("on_exit 1");
    }
    if (atexit(atexit_func1) != 0)
    {
        fatal("atexit 1");
    }
    if (atexit(atexit_func2) != 0)
    {
        fatal("atexit 2");
    }
    if (on_exit(onexit_func, (void *)20) != 0)
    {
        fatal("on_exit 2");
    }
    exit(2);
}