#define _GNU_SOURCE
#include <signal.h>
#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

static volatile int num_live_children = 0;

static void sigchld_handler(int sig)
{
    int status, saved_errno;
    pid_t child_pid;

    saved_errno = errno;

    printf("handler: Caught SIGCHLD\n");

    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("handler: Reaped child %ld - ", (long)child_pid);
        num_live_children--;
    }
    
    if (child_pid == -1 && errno != ECHILD)
        errMsg("waitpid");
    sleep(5);
    printf("handler: returning\n");

    errno = saved_errno;
}

int main(int argc, char *argv[])
{
    int j, sig_cnt;
    sigset_t block_mask, empty_mask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sig_cnt = 0;
    num_live_children = argc - 1;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1)
    {
        errExit("sigprocmask");
    }

    for (j = 1; j < argc; ++j)
    {
        switch (fork())
        {
        case -1:
            errExit("fork");

        case 0:
            sleep(getInt(argv[j], GN_NONNEG, "child-sleep-time"));
            printf("Child %d (PID=%ld) exiting\n", j, (long)getpid());
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }
    sigemptyset(&empty_mask);
    while (num_live_children > 0)
    {
        if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
            errExit("sigsuspend");
        sig_cnt++;
    }

    printf("All %d children have terminated; SIGHLD was caught %d times\n",
           argc-1, sig_cnt);
    exit(EXIT_SUCCESS);
}