#define _BSD_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include "tlpi_hdr.h"
int main(int argc, char *argv[])
{
    pid_t pid;

    switch (pid = vfork())
    {
    case -1:
        errExit("vfork");
    case 0:
        if (close(STDOUT_FILENO) == -1)
            errExit("close");
        assert(write(STDOUT_FILENO, "child-fuck\n", 12) == -1);
        _exit(EXIT_SUCCESS);
    default:
        write(STDOUT_FILENO, "parent-fuck\n", 13);
        exit(EXIT_SUCCESS);
    }
}