// 进程组首进程调用 setsid() 会失败
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include "tlpi_hdr.h"

int main(void)
{
    pid_t child_pid;
    // if (setpgid(0, 0) == -1)
    //     errExit("setpgid");

    // if (setsid() == -1)
    //     errExit("setsid");
    switch (child_pid = fork())
    {
    case -1:
        errExit("fork");
    case 0:
        printf("PID: %ld  PGID: %ld\n", (long)getpid(), (long)getpgrp());
        if (setpgid(0, 0) == -1)
            errExit("setpgid");
        signal(SIGTTOU, SIG_IGN);
        if (tcsetpgrp(STDOUT_FILENO, getpgrp()) == -1)
            errExit("tcsetpgrp");
        printf("fuck\n");
        if (setsid() == -1)
            errExit("setsid");
        printf("fuck\n");
        break;
    default:
        sleep(3);
    }
}