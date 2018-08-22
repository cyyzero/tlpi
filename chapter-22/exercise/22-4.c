#define _GNU_SOURCE
#include <signal.h>

#define NULL ((void*)0)

typedef void(*sighandler_t)(int);

// parameter disp can be SIG_HOLD or SIG_DEL or SIG_IGN
sighandler_t my_sigset(int sig, sighandler_t disp)
{
    sighandler_t ret;
    sigset_t mask;
    struct sigaction sa;

    sigprocmask(SIG_BLOCK, NULL, &mask);
    if (sigismember(&mask, sig))
    {
        ret = SIG_HOLD;
    }
    else
    {
        if (sigaction(sig, NULL, &sa) == -1)
            ret = (void (*)(int))-1;
        else
        {
            ret = sa.sa_handler;
        }
    }

    if (disp == SIG_HOLD)
    {
        if (sigemptyset(&mask) == -1)
            ret = (void (*)(int))-1;
        if (sigaddset(&mask, sig) == -1)
            ret = (void (*)(int))-1;
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
            ret = (void (*)(int))-1;
    }
    else
    {
        if (sigemptyset(&mask) == -1)
            ret = (void (*)(int))-1;
        if (sigaddset(&mask, sig))
            ret = (void (*)(int))-1;
        if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
            ret = (void (*)(int))-1;

        if (sigemptyset(&sa.sa_mask) == -1)
            ret = (void (*)(int))-1;
        if (sigaddset(&sa.sa_mask, sig) == -1)
            ret = (void (*)(int))-1;
        sa.sa_flags = 0;
        sa.sa_handler = disp;
        if (sigaction(sig, &sa, NULL))
            ret = (void (*)(int))-1;
    }

    return ret;
}

int sighold(int sig)
{
    int ret = 0;
    sigset_t mask;

    if (sigemptyset(&mask) == -1)
        ret = -1;

    if (sigaddset(&mask, sig) == -1)
        ret = -1;

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        ret = -1;
    return ret;
}

int sigrelse(int sig)
{
    int ret = 0;
    sigset_t mask;

    if (sigemptyset(&mask) == -1)
        ret = -1;

    if (sigaddset(&mask, sig) == -1)
        ret = -1;

    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
        ret = -1;
    return ret;
}

int sigignore(int sig)
{
    if (signal(sig, SIG_IGN) == SIG_ERR)
        return -1;
    else
        return 0;
}

int sigpause(int sig)
{
    sigset_t mask;

    //sigemptyset(&cur);
    //sigaddset(&cur, sig);
    sigprocmask(SIG_BLOCK, NULL, &mask);
    sigdelset(&mask, sig);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    return sigsuspend(&mask);
}

