#define _XOPEN_SOURCE
#define _BSD_SOURCE
#include <signal.h>
#include "tlpi_hdr.h"

static void tstp_handler(int sig)
{
    sigset_t tstp_mask, prev_mask;
    int saved_errno;
    struct sigaction sa;

    saved_errno = errno;

    printf("Caught SIGTSTP\n");

    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        errExit("signal");

    raise(SIGTSTP);

    sigemptyset(&tstp_mask);
    sigaddset(&tstp_mask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstp_mask, &prev_mask) == -1)
        errExit("sigprocmask");

    if (sigprocmask(SIG_SETMASK, &prev_mask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstp_handler;
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Exiting SIGTSTP handler\n");
    errno = saved_errno;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    if (sigaction(SIGTSTP, NULL, &sa) == -1)
        errExit("sigaction");

    if (sa.sa_handler != SIG_IGN)
    {
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = tstp_handler;
        if (sigaction(SIGTSTP, &sa, NULL) == -1)
            errExit("sigaction");
    }

    for (;;)
    {
        pause();
        printf("Main\n");
    }
}