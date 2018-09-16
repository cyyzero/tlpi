#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include "tlpi_hdr.h"


int main(int argc, char *argv[])
{
    int sched_kind;
    struct sched_param sp;

    if (strncmp(argv[1], "r", 1))
    {
        sched_kind = SCHED_RR;
    }
    else if (strncmp(argv[1], "f", 1))
    {
        sched_kind = SCHED_FIFO;
    }

    sp.sched_priority = getInt(argv[2], 0, "sched_priority");
    if (sched_setscheduler(0, sched_kind, &sp) == -1)
        errExit("sched_setscheduler");
    if (execvp(argv[3], argv+3) == -1)
        errExit("execvp");
}