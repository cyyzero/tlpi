#define _GNU_SOURCE
#include <signal.h>
#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1

static void handler(int sig)
{
}

int main(int argc, char *argv[])
{
    pid_t child_pid;
    sigset_t block_mask, orig_mask, empty_mask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SYNC_SIG);
    if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (child_pid = fork())
    {
    case -1:
        errExit("fork");
    case 0:
        printf("[%ld] Child started - doing some work\n", (long)getpid());
        //sleep(2);

        printf("[%ld] Child about to signal parent\n", (long)getpid());

        if (kill(getppid(), SYNC_SIG) == -1)
            errExit("kill");
    
        _exit(EXIT_SUCCESS);
    default:
        sleep(2);
        printf("[%ld] Parent about to wait for signal\n", (long)getpid());
        sigemptyset(&empty_mask);
        if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
            errExit("sigsuspend");
        printf("[%ld] Parent got singal\n", (long)getpid());

        if (sigprocmask(SIG_SETMASK, &orig_mask, NULL) == -1)
            errExit("sigprocmask");
        exit(EXIT_SUCCESS);
    }
}