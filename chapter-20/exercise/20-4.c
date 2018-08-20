#define _GNU_SOURCE
#include <signal.h>

int my_siginterrupt(int sig, int flag)
{
    int ret;
    struct sigaction act;
    sigaction(sig, NULL, &act);

    if (flag)
        act.sa_flags &= SA_RESTART;
    else
        act.sa_flags |= SA_RESTART;
    ret = sigaction(sig, &act, NULL);
    return ret;
}