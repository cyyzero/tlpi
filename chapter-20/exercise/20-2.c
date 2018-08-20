#define _GNU_SOURCE
#include <signal.h>
#include <string.h>
#include "tlpi_hdr.h"

static int flag = 0;

void handler(int sig)
{
    printf("SIG %d is caught.\n", sig);
    if (sig == SIGINT)
        flag = 1;
}

int main(int argc, char *argv[])
{
    sigset_t set;

    for (int i = 1; i < NSIG; ++i)
        signal(i, handler);

    while (flag == 0)
        pause();

    printf("----------\n");

    flag = 0;
    // if (sigfillset(&set) == -1)
    //     errExit("sigfillset");
    // if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
    //     errExit("sigprocmask");

    // sleep(10);

    // sigpending(&set);

    // for (int i = 1; i < NSIG; ++i)
    // {
    //     if (sigismember(&set, i))
    //         printf("%d is blocked.\n");
    // }

    // if (sigemptyset(&set) == -1)
    //     errExit("signempty");

    // sigprocmask(SIG_SETMASK, &set, NULL);
    for (int i = 1; i < NSIG; ++i)
        signal(i, SIG_IGN);
    pause();
    // sleep(10);
    return 0;
}