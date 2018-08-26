#define _XOPEN_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include "tlpi_hdr.h"

void handler(int sig)
{
    if (sig == SIGUSR2)
        printf("Parent has been exited\n");
}

int main(void)
{
    sigset_t mask;
    struct sigaction sa;
    pid_t child_pid, ppid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
        errExit("sigaction");

    sigemptyset(&mask);

    switch (child_pid = fork())
    {
    case -1:
        errExit("fork");
    
    case 0:
        ppid = getppid();
        prctl(PR_SET_PDEATHSIG, SIGUSR2);
        printf("Child PID is %ld\n", (long)getpid());
        printf("Parent PID is %ld\n", (long)ppid);
        if (kill(ppid, SIGUSR1) == -1)
            errExit("kill");
        sigsuspend(&mask);
        printf("Now parent PID is %ld\n", (long)getppid());

    default:
        sigsuspend(&mask);
        exit(EXIT_SUCCESS);
    }
}