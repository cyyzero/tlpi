#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int status;
    pid_t child_pid;

    switch (fork())
    {
    case -1:
        errExit("fork");

    case 0:
        printf("Child started with PID = %ld\n", (long)getpid());
        if (argc > 1)
            exit(getInt(argv[1], 0, "exit-status"));
        else
            for(;;)
                pause();
        exit(EXIT_SUCCESS);

    default:
        for (;;)
        {
            child_pid = waitpid(-1, &status, WUNTRACED
#ifdef WCONTINUED
                                             | WCONTINUED
#endif
            );
            if (child_pid == -1)
                errExit("waitpid");
            printf("waitpid() returned: PID=%ld;status=0x%04d (%d,%d)\n",
                   (long)child_pid, (unsigned int)status, status >> 8, status & 0xff);
            print_wait_status(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                exit(EXIT_SUCCESS);
        }
    }
}