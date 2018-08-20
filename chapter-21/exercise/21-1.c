#define _XOPEN_SOURCE
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include "tlpi_hdr.h"

void my_abort(void)
{
    // unblock the SIGABRT signal.
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGABRT);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    // Raise SIGABRT
    raise(SIGABRT);

    // If the SIGABRT signal is ignored, or caught by a handler that  returns,
    // the abort() function will still terminate the process.  It does this by
    // restoring the default disposition for SIGABRT and then raising the sig‐
    // nal for a second time.
    signal(SIGABRT, SIG_DFL);
    raise(SIGABRT);
}

sigjmp_buf env;

void handler_not_return(int sig)
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
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGABRT);
    sigprocmask(SIG_BLOCK, &set, NULL);
    printf("SIGABRT is blocked!");
    raise(SIGABRT);
    printf("SIGABRT has been raised.\n");
    if (sigsetjmp(env, 1) == 0)
    {
        printf("sigsetjmp called\n");
        if (signal(SIGABRT, handler_not_return) == SIG_ERR)
            errExit("signal");
    }
    else
    {
        printf("Return from abort\n");
        if (signal(SIGABRT, handler_return) == SIG_ERR)
            errExit("signal");
    }

    my_abort();
    printf("Call abort!\n");
}