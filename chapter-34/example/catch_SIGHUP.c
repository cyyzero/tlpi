#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void handler(int sig)
{
}

int main(int argc, char *argv[])
{
    pid_t child_pid;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");

    child_pid = fork();
    if (child_pid == -1)
        errExit("fork");

    if (child_pid == 0 && argc > 1)
    {
        if (setpgid(0, 0) == -1)
            errExit("setpgid");
    }

    printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n", (long)getpid(),
           (long)getppid(), (long)getpgrp(), (long)getsid(0));

    alarm(30);

    for (;;)
    {
        pause();
        printf("%ld: caught SIGHUP\n", (long)getpid());
    }
}