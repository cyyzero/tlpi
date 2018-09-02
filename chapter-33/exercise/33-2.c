/*
* 在我机器上(Linux kernel 4.14.0-041400-generic, gcc 8.1.0)测试的时候，
* 某个线程A fork() 之后子进程退出，SIGCHLD默认是先传给线程A。
* 当在线程A中把 SIGCHLD mask 之后，会传递给其他未屏蔽 SIGCHLD 的线程。
*/
#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include "tlpi_hdr.h"

volatile pthread_t caught_sig_thread;

void handler(int sig)
{
    if (sig == SIGCHLD)
    {
        caught_sig_thread = pthread_self();
        printf("[PID %llu] SIGCHLD is caught\n", (unsigned long long)caught_sig_thread);
    }
}

void *thread_func(void *arg)
{
    printf("Thread t1 id %llu\n", (unsigned long long)pthread_self());
    int status;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    switch (fork())
    {
    case -1:
        errExit("fork");
    case 0:
        _exit(EXIT_SUCCESS);
    default:
        if (wait(&status) == -1)
            errExit("wait");
    }
}

int main(int argc, char *argv[])
{
    for (int i = 0; ; ++i)
    {
        int s;
        struct sigaction sa;
        pthread_t t1;

        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = handler;
        sigaction(SIGCHLD, &sa, NULL);
        printf("Main thread id %llu\n", (unsigned long long)pthread_self());
        s = pthread_create(&t1, NULL, thread_func, NULL);
        if (s != 0)
            errExitEN(s, "pthread_create");

        s = pthread_join(t1, NULL);
        if (s != 0)
            errExitEN(s, "pthread_join");
        
        if (pthread_equal(caught_sig_thread, pthread_self()))
        {
            printf("%d\n", i);
        }
    }
}