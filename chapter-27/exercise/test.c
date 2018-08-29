#define _XOPEN_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

static volatile sig_atomic_t has_sig = 0;

void handler(int sig)
{
    has_sig = 1;
}


int main(void)
{
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    system("ls -ahl");

    if (has_sig == 1)
    {
        printf("SIGCHLD has been caught.\n");
    }
}
