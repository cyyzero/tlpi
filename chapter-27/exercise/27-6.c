/*
* 父进程阻塞 SIGCHLD 信号，并wait子进程。子进程退出仍会发送 SIGCHLD 信号。
* 这意味着当前进程调用 system() 后，会收到一个 SIGCHLD 信号，需要程序猿对这个信号正确地处理。
* 可以试着编译 test.c 查看运行结果。
*/
#define _XOPEN_SOURCE
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

void handler(int sig)
{
    printf("SIGCHLD is caught!\n");
}
int main(void)
{
    int status;
    sigset_t mask;
    struct sigaction sa;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    sigaction(SIGCHLD, &sa, NULL);

    switch (fork())
    {
    case -1:
        errExit("fork");

    case 0:
        _exit(EXIT_SUCCESS);

    default:
        if (wait(&status) == -1)
            errExit("wait");

        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        exit(EXIT_SUCCESS);
    }
}