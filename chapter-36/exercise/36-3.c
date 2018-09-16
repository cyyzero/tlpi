// 感觉根据资源类型而改变？　进程　RLIMIT_NPROC　超出范围会导致fork()失败，并设置errno为 EAGAIN
#include <stdio.h>
#include <sys/resource.h>

#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct rlimit sr = {1, 1};

    if (setrlimit(RLIMIT_NPROC, &sr) == -1)
        errExit("setrlimit");

    while (1)
    {
        if (fork() == -1)
            errExit("fork");

        printf("Fuck\n");
    }
}