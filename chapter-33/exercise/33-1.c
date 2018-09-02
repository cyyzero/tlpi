#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include "tlpi_hdr.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static volatile int flag = 0;

void print_pendding_signal(const char *msg)
{
    sigset_t set;

    sigpending(&set);
    printf("%s:", msg);
    for (int i = 1; i < NSIG; ++i)
    {
        if (sigismember(&set, i))
            printf("%d ", i);
    }
    printf("\n");
}

void *thread_func(void *arg)
{
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    pthread_mutex_lock(&mtx);
    while (flag == 0)
    {
        pthread_cond_wait(&cond, &mtx);
    }
    print_pendding_signal("Tthread t1");
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(int argc, char *argv[])
{
    sigset_t mask;
    pthread_t t1;
    int s;

    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    pthread_kill(pthread_self(), SIGQUIT);

    s = pthread_create(&t1, NULL, thread_func, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");
    sleep(2);
    pthread_kill(t1, SIGQUIT);
    pthread_kill(t1, SIGINT);
    flag = 1;
    pthread_cond_signal(&cond);
    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    print_pendding_signal("Main thread");

    exit(EXIT_SUCCESS);
}