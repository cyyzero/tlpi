#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    pol = (argv[1][0] == 'r') ? SCHED_RR :
            (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH
            (argv[1][0] == 'b') ? SHCED_BATCH;
#endif
#ifdef SHCED_IDLE
            (argv[1][0] == 'i') ? SCHED_IDLE:
#endif
            SCHED_OTHER;
    
    for (j = 3; j < argc; ++j)
    {
        if (sched_setscheduler(getLong(argv[j], 0, "pid"), pol, &sp) == -1)
            errExit("sched_setscheduler");
    }
    exit(EXIT_FAILURE);
}