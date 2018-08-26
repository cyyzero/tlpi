#define _XOPEN_SOURCE
#include <signal.h>
#include <libgen.h>
#include "tlpi_hdr.h"

#define CMD_SIZE 200

void handler(int sig)
{
}

int main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t child_pid;
    sigset_t mask;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGCHLD, &sa, NULL))
        errExit("sigaction");

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    sigfillset(&mask);
    sigdelset(&mask, SIGCHLD);

    setbuf(stdout, NULL);

    printf("Parent PID=%ld\n", (long)getpid());

    switch (child_pid = fork())
    {
    case -1:
        errExit("fork");

    case 0:
        //printf("Dfas\n");
        sleep(1);
        printf("Child (PID=%ld) exiting\n", (long)getpid());
        _exit(EXIT_SUCCESS);

    default:
        //sleep(3);
        sigsuspend(&mask);
        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
        cmd[CMD_SIZE-1] = '\0';
        system(cmd);

        if (kill(child_pid, SIGKILL) == -1)
            errMsg("kill");
        //sleep(3);
        printf("After sending SIGKILL to zombie (PID=%ld):\n", (long)child_pid);
        system(cmd);
        exit(EXIT_SUCCESS);
    }
}