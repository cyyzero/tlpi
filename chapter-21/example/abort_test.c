#define _XOPEN_SOURCE
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include "tlpi_hdr.h"

sigjmp_buf env;

void handler(int sig)
{
    printf("SIGABORT is caugnt!\n");

    siglongjmp(env, 1);
}

void handler_return(int sig)
{
    printf("SIGABORT is caught! It will return.\n");
}

int main(void)
{
    if (sigsetjmp(env, 1) == 0)
    {
        printf("sigsetjmp called\n");
        if (signal(SIGABRT, handler) == SIG_ERR)
            errExit("signal");
    }
    else
    {
        printf("Return from abort\n");
        if (signal(SIGABRT, handler_return) == SIG_ERR)
            errExit("signal");
    }



    abort();
    printf("Call abort!\n");
}