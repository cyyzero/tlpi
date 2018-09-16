#include <sys/resource.h>
#include "print_rlimit.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct rlimit rl;
    int j;
    pid_t child_pid;

    print_rlimit("Initial maximun process limits: ", RLIMIT_NPROC);

    rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY :
                                getInt(argv[1], 0, "soft-limit");

    rl.rlim_max = (argc > 2) ? rl.rlim_cur :
                    (argv[2][0] == 'i') ? RLIM_INFINITY:
                    getInt(argv[2], 0, "hard-limit");

    if (setrlimit(RLIMIT_NPROC, &rl) == -1)
        errExit("setrlimit");

    print_rlimit("New maximun process liits:    ", RLIMIT_NPROC);

    for (j = 1; ; ++j)
    {
        switch (child_pid = fork())
        {
        case -1:
            errExit("fork");
        case 0:
            _exit(EXIT_SUCCESS);
        default:
            printf("Child %d (PID=%ld) started\n", j, (long)child_pid);
            break;
        }
    }
}