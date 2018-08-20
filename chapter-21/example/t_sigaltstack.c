#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void sigsegv_handler(int sig)
{
    int x;

    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    printf("Top of hanler stack near %10p\n", (void *)&x);
    fflush(NULL);

    _exit(EXIT_FAILURE);
}

static void overflow_stack(int call_num)
{
    char a[100000];

    printf("Call %4d - top of stack near %10p\n", call_num, &a[0]);
    overflow_stack(call_num + 1);
}

int main(int argc, char *argv[])
{
    stack_t sigstack;
    struct sigaction sa;
    int j;

    printf("Top of standard stack is near %10p\n", (void *)&j);

    sigstack.ss_sp = malloc(SIGSTKSZ);
    if (sigstack.ss_sp == NULL)
        errExit("malloc");
    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if (sigaltstack(&sigstack, NULL) == -1)
        errExit("sigaltstack");
    printf("Alternate stack is at %10p-%p\n", sigstack.ss_sp, (char *)sbrk(0)-1);

    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    //sa.sa_flags = 0;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        errExit("sigaction");

    overflow_stack(1);
}