#define _BSD_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "tlpi_hdr.h"

static int *pids;

FILE *_popen(const char *command, const char *type)
{
    int pfd[2];
    int fds;
    FILE *fp;

    if (type[0] != 'r' && type[0] != 'w' || type[1])
        return NULL;

    if (pids == NULL)
    {
        fds = getdtablesize();
        if ((pids = calloc(fds, sizeof(int))) == NULL)
            return NULL;
    }

    if (pipe(pfd) == -1)
        return NULL;

    switch (fork())
    {
    case -1:
        close(pfd[0]);
        close(pfd[1]);
        return NULL;

    case 0:
        if (*type == 'r')
        {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO)
            {
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        }
        else
        {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO)
            {
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }

        execlp("/bin/sh", "/bin/sh", "-c", command, NULL);
        _exit(127);

    default:
        if (*type == 'r')
        {
            close(pfd[1]);
            if ((fp = fdopen(pfd[0], "r")) == NULL)
                return NULL;
            pids[pfd[0]] = getpid();
        }
        else
        {
            close(pfd[0]);
            if ((fp = fdopen(pfd[1], "w")) == NULL)
                return NULL;
            pids[pfd[1]] = getpid();
        }
        return fp;
    }
}

int _pclose(FILE *stream)
{
    int status;
    int fd;

    if (pids == NULL)
        return -1;

    if ((fd = fileno(stream)) == -1)
        return -1;

    if (waitpid(pids[fd], &status, 0) == -1)
        return -1;
    pids[fd] = 0;
    return 1;
}


int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[100];
    
    
    fp = _popen("ls -ahl", "r");

    while (fgets(buf, 100, fp) != NULL)
    {
        fputs(buf, stdout);
    }
    return 0;
}
