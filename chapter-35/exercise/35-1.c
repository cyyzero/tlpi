#define _GNU_SOURCE
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv[])
{

    int prio;

    prio = getpriority(PRIO_PROCESS, 0);

    if (prio == -1)
        errExit("getpriority");

    if (argc == 1)
    {
        printf("%d\n", prio);
    }
    else
    {
        int opt, flag, add_nice;
        while ((opt = getopt(argc, argv, "n:")) != -1)
        {
            switch (opt)
            {
            case 'n':
                flag = 1;
                add_nice = getInt(optarg, 0, "add_nice");
                break;
            default:
                fatal("Unknown args");
            }
        }

        if (optind < argc-1)
            fatal("Error\n");

        if (setpriority(PRIO_PROCESS, 0, prio + add_nice) == -1)
            errExit("setpriority");

        if (execvp(argv[optind], argv+optind) == -1)
            errExit("execve");
    }
}