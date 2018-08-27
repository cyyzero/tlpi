#define _XOPEN_SOURCE
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int system(const char *command)
{
    sigset_t block_mask, orig_mask;
    struct sigaction sa_ignore, sa_orig_quit, sa_orig_int, sa_default;
    pid_t child_pid;
    int status, saved_errno;

    if (command == NULL)
        return system(":") == 0;

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);

    sa_ignore.as_handler = SIG_IGN;
    sa_ignore.sa_flags = 0;
    sigemptyset(sa_ignore.sa_mask);
    sigaction(SIGINT, &sa_ignore, &sa_orig_int);
    sigaction(SIGQUIT, &sa_ignore, &sa_orig_quit);

    switch(child_pid = fork())
    {
    case -1:
        status = -1;
        break;

    case 0:
        sa_default.sa_handler = SIG_DFL;
        sa_default.sa_flags = 0;
        sigemptyset(&sa_default.sa_mask);

        if (sa_orig_int.sa_handler != SIG_IGN)
            sigaction(SIGINT, &sa_default, NULL);
        if (sa_orig_quit.sa_handler != SIG_IGN)
            sigaction(SIGQUIT, &sa_default, NULL);

        sigprocmask(SIG_SETMASK, &orig_mask, NULL);

        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(127);

    default:
        while (waitpid(child_pid, &status, 0) == -1)
        {
            if (errno != EINTR)
            {
                status = -1;
                break;
            }
        }
        break;
    }

    saved_errno = errno;

    sigprocmask(SIG_SETMASK, &orig_mask, NULL);
    sigaction(SIGINT, &sa_orig_int, NULL);
    sigaction(SIGQUIT, &sa_orig_quit, NULL);

    errno = saved_errno;

    return status;
}