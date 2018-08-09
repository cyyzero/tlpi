#include <sys/times.h>
#include <time.h>
#include "tlpi_hdr.h"

static void display_process_times(const char *msg)
{
    struct tms t;
    clock_t clock_time;
    static long clock_ticks = 0;

    if (msg != NULL)
        printf("%s", msg);
        
    if (clock_ticks == 0)
    {
        clock_ticks = sysconf(_SC_CLK_TCK);
        if (clock_ticks == -1)
            errExit("sysconf");
    }

    clock_time = clock();

    if (clock_time == -1)
        errExit("clock");

    printf("    clock() returns: %ld clock-per-sec (%.2f secs)\n",
           (long)clock_time, (double)clock_time/CLOCKS_PER_SEC);

    if (times(&t) == -1)
        errExit("times");
    printf("    times() yields: user cpu=%.2f; system CPU: %.2f\n",
           (double)t.tms_utime/clock_ticks,
           (double)t.tms_stime/clock_ticks);
}

int main(int argc, char *argv[])
{
    int num_cells, j;

    printf("CLOCKS_PER_SEC=%ld  sysconf(_SC_CLK_TCK)=%ld\n\n",
           (long)CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

    display_process_times("At program start:\n");

    num_cells = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-calls") : 100000000;
    for (j = 0; j < num_cells; ++j)
        (void)getppid();
    display_process_times("After getppid() loop:\n");

    exit(EXIT_SUCCESS);
}