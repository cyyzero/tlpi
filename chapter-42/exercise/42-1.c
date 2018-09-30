#include <stdio.h>
#include <dlfcn.h>
#include <wait.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main(void)
{
    int status;
    void  *parent_ret;
    pid_t child_pid;
    
    switch(child_pid = fork())
    {
    case -1:
        errExit("fork()");
    case 0:
        pause();
    default:
        parent_ret = dlopen("libc.so.6", RTLD_NOW);
        if (parent_ret == NULL)
            fatal("dlopen: %s", dlerror());
        assert(dlclose(parent_ret) == 0);
        assert(dlopen("libc.so.6", RTLD_LAZY | RTLD_NOLOAD) != NULL);
        //if (parent_ret == NULL)
            // fatal("dlopen: %s", dlerror());
        printf("libc.so.6 is still in memory\n");
        kill(child_pid, SIGINT);
        wait(&status);
    }
}