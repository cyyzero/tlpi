#define _XOPEN_SOURCE 500
#include <sys/wait.h>
//#include "print_wait_status.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int status;
    pid_t child_pid;
    siginfo_t info;

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
            child_pid = waitid(P_ALL, 0, &info, WSTOPPED
#ifdef WCONTINUED
                                                | WCONTINUED
#endif
            );
            if (child_pid == -1)
                errExit("waitpid");
            
            printf("waitpid() returned: PID=%ld UID=%ld status=0x%04d (%d,%d) code is ",
                   (long)info.si_pid, (long)info.si_uid, 
                   (unsigned int)status, status >> 8, status & 0xff);
            switch (info.si_code)
            {
            case CLD_EXITED: printf("CLD_EXITED\n"); break;
            case CLD_KILLED: printf("CLD_KILLED\n"); break;
            case CLD_DUMPED: printf("CLD_DUMPED\n"); break;
            case CLD_STOPPED: printf("CLD_STOPPED\n"); break;
            case CLD_TRAPPED: printf("CLD_TRAPPED\n"); break;
            case CLD_CONTINUED: printf("CLD_CONTINUED\n"); break;
            default: fatal("fuck");
            }
            //print_wait_status(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                exit(EXIT_SUCCESS);
        }
    }
}