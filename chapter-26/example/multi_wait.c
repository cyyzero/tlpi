#include <sys/wait.h>
#include <time.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int num_dead;
    pid_t child_pid;
    int j;

    setbuf(stdout, NULL);

    for (j = 1; j < argc; ++j)
    {
        switch (fork())
        {
        case -1:
            errExit("fork");

        case 0:
            printf("Child %d started with PID %ld, sleeping %s seconds\n", 
                   j, (long)getpid(), argv[j]);
            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    num_dead = 0;
    for (;;)
    {
        child_pid = wait(NULL);
        if (child_pid == -1)
        {
            if (errno == ECHILD)
            {
                printf("No more children - bytes\n");
                exit(EXIT_SUCCESS);
            }
            else
            {
                errExit("wait");
            }
        }

        num_dead++;
        printf("wait() returned child PID %ld (num_dead=%d)\n", (long)child_pid, num_dead);
    }
}