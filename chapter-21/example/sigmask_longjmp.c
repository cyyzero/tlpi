#define _GNU_SOURCE
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include "signal_functions.h"
#include "tlpi_hdr.h"

static volatile sig_atomic_t can_jmp = 0;

#ifdef USE_SIGSETJMP
static sigjmp_buf senv;
#else
static jmp_buf env;
#endif

static void handler(int sig)
{
    printf("Received signal %d (%s), signal mask is:\n", sig, strsignal(sig));
    
    print_sig_mask(stdout, NULL);

    if (!can_jmp)
    {
        printf("'env' buffer not yet set, doing a simple return\n");
        return;
    }

#ifdef USE_SIGSETJMP
    siglongjmp(senv, 1);
#else
    longjmp(env, 1);
#endif // DEBUG
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    print_sig_mask(stdout, "Signal mask at startup:\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

#ifdef USE_SIGSETJMP
    printf("Calling sigsetjmp()\n");
    if (sigsetjmp(senv, 1) == 0)
#else
    printf("Calling setjmp()\n");
    if (setjmp(env) == 0)
#endif
        can_jmp = 1;
    else
        print_sig_mask(stdout, "After jump from handler, signal mask is:\n");

    for (;;)
        pause();
}