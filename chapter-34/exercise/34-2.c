// 父进程能够在子进程执行 exec() 之前修改子进程的进程 ID， 但是无法在执行 exec() 之后修改
// 子进程的进程组 ID
#include <signal.h>
#include <stdio.h>
#include "tlpi_hdr.h"

int main(void)
{
    pid_t child_pid;

    switch (child_pid = fork())
    {
    case -1:
        errExit("fork");

    case 0:
        sleep(3);
        if (execl("./a.out", "./a.out", NULL) == -1)
            errExit("execl");
        
    default:
        //sleep(3);
        if (setpgid(child_pid, child_pid) == -1)
            errExit("setpgid");
    }
}