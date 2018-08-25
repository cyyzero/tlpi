/*
* WEXITSTATUS(status)
* returns  the  exit  status  of  the child.  This consists of the
* least significant 8 bits of the status argument that  the  child
* specified  in  a  call to exit(3) or _exit(2) or as the argument
* for a return statement in main().  This macro should be employed
* only if WIFEXITED returned true.
*/
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;

    switch (pid = fork())
    {
    case -1:
        errExit("fork");
    case 0:
        exit(-1);
    default:
        if (wait(&status) == -1)
            errExit("wait");
        if (WIFEXITED(status))
            printf("0x%x\n", WEXITSTATUS(status));   // Output 0xff
        else
            errExit("WIFEXITED");
        exit(EXIT_SUCCESS);
    }
}