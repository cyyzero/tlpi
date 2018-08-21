#define _GNU_SOURCE
#include <sys/signalfd.h>
#include <signal.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    sigset_t mask;
    int sfd, j;
    struct signalfd_siginfo fdsi;
    ssize_t s;

    printf("PID = %ld\n", (long)getpid());

    sigemptyset(&mask);
    for (j = 1; j < argc; ++j)
        sigaddset(&mask, atoi(argv[j]));

    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1)
        errExit("signalfd");

    for (;;)
    {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo))
            errExit("read");

        printf("got signal %d", fdsi.ssi_signo);
        if (fdsi.ssi_code == SI_QUEUE)
        {
            printf("; ssi_pid=%d;ssiint=%d", fdsi.ssi_pid, fdsi.ssi_int);
        }
        printf("\n");
    }
}