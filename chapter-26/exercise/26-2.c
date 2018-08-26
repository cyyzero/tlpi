/*
* 经过测试，对于祖父，父以及子进程，父进程变成僵尸进程，孙进程在父进程终止后就会
* 被 init 进程收养，而不是祖父进程调用 wait() 后。
*/
#define _XOPEN_SOURCE
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>
#include "tlpi_hdr.h"

void handler(int sig)
{
}

int main(int argc, char *argv[])
{
    sigset_t mask;
    struct sigaction sa;
    pid_t grandparent, parent, son;
    int status;

    grandparent = getpid();

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");

    sigemptyset(&mask);

    setbuf(stdout, NULL);

    switch (parent = fork())
    {
    case -1:
        errExit("fork");
    case 0:
        switch (son = fork())
        {
        case -1:
            errExit("fork");
        case 0:
            while (kill(getppid(), SIGUSR1) == 0)
                continue;
            printf("After parent process exites, parent PID is %ld\n", (long)getppid());
            kill(grandparent, SIGUSR1);
            while (kill(grandparent, SIGUSR1) == 0)
                continue;
            printf("After grandparent process waits, parent PID is %ld\n", (long)getppid());
            exit(EXIT_SUCCESS);
        default:
            printf("[%ld] Exit...\n", (long)getpid());
            _exit(EXIT_SUCCESS);
        }

    default:
        sigsuspend(&mask);
        printf("[%ld] Wait...\n", (long)getpid());
        if (wait(&status) == -1)
            errExit("wait");
        exit(EXIT_SUCCESS);
    }

}