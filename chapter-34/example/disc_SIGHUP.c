#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void handler(int sig)
{
    printf("PID %ld: caught signal %2d (%s)\n", (long)getpid(), sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    pid_t parent_pid, child_pid;
    int j;
    struct sigaction sa;

    setbuf(stdout, NULL);

    parent_pid = getpid();
    printf("PID of parent is: %ld\n", (long)getppid());
    printf("Foreground process group ID is: %ld\n", (long)tcgetpgrp(STDIN_FILENO));

    for (j = 1; j < argc; ++j)
    {
        child_pid = fork();
        if (child_pid == -1)
        {
            errExit("fork");
        }

        if (child_pid == 0)
        {
            if (argv[j][0] == 'd')
            {
                if (setpgid(0, 0) == -1)
                    errExit("setpgid");
            }

            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1)
                errExit("sigaction");
            break;
        }
    }

    alarm(60);

    printf("PID=%ld PGID=%ld\n", (long)getpid(), (long)getppid());
    for (;;)
    {
        pause();
    }
}