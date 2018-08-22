// 经过验证，linux上对于信号的处理顺序是按照信号的数值顺序来的。
// 并且，在阻塞阶段非实时信号只接受一次，实时信号接收多次

#define _GNU_SOURCE
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "tlpi_hdr.h"

#define rand_signo (rand()%(NSIG-1) + 1)

int send_sigs[200];
int send_sigs_index = 0;
int receive_sigs[200];
int receive_sigs_index = 0;

static void handler(int sig)
{
    receive_sigs[receive_sigs_index++] = sig;
    //printf("Caught signal %d\n", sig);
}

int main(void)
{
    sigset_t set;
    struct sigaction sa;

    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    for (int i = 1; i < NSIG; ++i)
    {
        if (i == SIGKILL || i == SIGSTOP || i == 32 || i == 33)
            continue;
        if (sigaction(i, &sa, NULL) == -1)
        {
            //printf("%d\n", i);
            errExit("sigaction");
        }
        //printf("%d\n", i);
    }

    srand(time(NULL));

    if (sigfillset(&set) == -1)
        errExit("sigfillset");
    if (sigprocmask(SIG_BLOCK, &set, NULL))
        errExit("sigprocmask");

    for (int i = 0; i < 200; ++i)
    {
        int sig = rand_signo;
        if (sig == SIGKILL || sig == SIGSTOP || sig == 32 || sig == 33)
            continue;
        //printf("%d \n", sig);
        if (raise(sig) == -1)
            errExit("raise");
        send_sigs[send_sigs_index++] = sig;
    }

    if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1)
        errExit("sigprocmask");

    printf("Send sigs:\n");
    for (int i = 0; i < send_sigs_index; ++i)
        printf("%d ", send_sigs[i]);
    printf("\n");

    printf("Receive sigs:\n");
    for (int i = 0; i < receive_sigs_index; ++i)
        printf("%d ", receive_sigs[i]);
    printf("\n");
    exit(EXIT_SUCCESS);

}