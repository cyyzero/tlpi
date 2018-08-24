#define _GNU_SOURCE
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

static volatile sig_atomic_t got_alarm = 0;

static void display_times(const char *msg, bool inlcude_timer)
{
    struct itimerval itv;
    static struct timeval start;
    struct timeval curr;
    static int call_num = 0;

    if (call_num == 0)
    {
        if (gettimeofday(&start, NULL) == -1)
            errExit("gettimeofday");
    }

    if (call_num % 20 == 0)
    {
        printf("    Elapsed Value Interval\n");
    }
    if (gettimeofday(&curr, NULL) == -1)
        errExit("gettimeofday");
    printf("%-7s %6.2f", msg, curr.tv_sec - start.tv_sec + 
           (curr.tv_usec - start.tv_usec) / 1000000.0);

    if (inlcude_timer)
    {
        if (getitimer(ITIMER_REAL, &itv) == -1)
            errExit("getitimer");
        printf("  %6.2f  %6.2f", 
               itv.it_value.tv_sec + itv.it_value.tv_usec/1000000.0,
               itv.it_interval.tv_sec + itv.it_interval.tv_usec/1000000.0);
    }

    printf("\n");

}

static void sigalrm_handler(int sig)
{
    got_alarm = 1;
}

int main(int argc, char *argv[])
{
    struct itimerval itv;
    clock_t prev_clock;
    int max_sigs;
    int sig_cnt;
    struct sigaction sa;

    sig_cnt = 0;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrm_handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    max_sigs = (itv.it_interval.tv_sec == 0 && itv.it_interval.tv_usec == 0) ?
               1 : 3;

    display_times("START:", false);

    itv.it_value.tv_sec  = (argc > 1) ? getLong(argv[1], 0, "secs") : 2;
    itv.it_value.tv_usec = (argc > 2) ? getLong(argv[2], 0, "usecs"): 0;
    itv.it_interval.tv_sec  = (argc > 3) ? getLong(argv[3], 0, "int-secs") : 0;
    itv.it_interval.tv_usec = (argc > 4) ? getLong(argv[4], 0, "int-usecs"): 0;

    if (setitimer(ITIMER_REAL, &itv, 0) == -1)
        errExit("setitimer");

    prev_clock = clock();
    sig_cnt = 0;

    for (;;)
    {
        while (((clock() - prev_clock) * 10 / CLOCKS_PER_SEC) < 5)
        {
            if (got_alarm)
            {
                got_alarm = 0;
                display_times("ALARM:", true);

                sig_cnt++;
                if (sig_cnt > max_sigs)
                {
                    printf("That's all folks\n");
                    exit(EXIT_SUCCESS);
                }
            }
        }

        prev_clock = clock();
        display_times("Main:", true);
    }
}