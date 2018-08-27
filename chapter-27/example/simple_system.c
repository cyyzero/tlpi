#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int system(char *command)
{
    int status;
    pid_t child_pid;

    switch (child_pid = fork())
    {
    case -1:
        return -1;

    case 0:
        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(127);

    default:
        if (waitpid(child_pid, &status, 0) == -1)
            return -1;
        else
            return status;
    }
}