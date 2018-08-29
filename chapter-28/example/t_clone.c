#define _GNU_SOURCE
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include "tlpi_hdr.h"

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

static int child_func(void *arg)
{
    if (close(*((int *)arg)) == -1)
        errExit("close");
    return 0;
}

int main(int argc, char *argv[])
{
    const int STACK_SIZE = 65536;
    char *stack;
    char *stack_top;
    int s, fd, flags;

    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
        errExit("open");

    flags = (argc > 1) ? CLONE_FILES : 0;

    stack = malloc(STACK_SIZE);
    if (stack == NULL)
        errExit("malloc");
    stack_top = stack + STACK_SIZE;

    if (CHILD_SIG != 0 && CHILD_SIG != SIGCHLD)
        if (signal(CHILD_SIG, SIG_IGN) == SIG_ERR)
            errExit("signal");

    if (clone(child_func, stack_top, flags | CHILD_SIG, (void *)&fd) == -1)
        errExit("clone");

    if (waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1)
        errExit("waitpid");
    printf("Child has terminated.\n");

    s = write(fd, "x", 1);
    if (s == -1 && errno == EBADF)
    {
        printf("File descriptor %d has been closed.\n", fd);
    }
    else if (s == -1)
    {
        printf("write() on file descriptor %d failed unexceptedly (%s)\n", fd, strerror(errno));
    }
    else
    {
        printf("write() on file descriptor %d succeeded\n", fd);
    }
    exit(EXIT_SUCCESS);
}