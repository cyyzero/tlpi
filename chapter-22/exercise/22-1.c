#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int sig)
{
    printf("SIGCONT is caught");
    exit(EXIT_SUCCESS);
}

int main(void)
{
    int flag;
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    sa.sa_flags = 0;

    sigaction(SIGCONT, &sa, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGCONT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    for (;;)
    {
        scanf("%d", &flag);
        if (flag == 1)
        {
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
    }
}