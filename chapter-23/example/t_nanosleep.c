#define _POSIX_C_SOURCE 199309
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void sigint_handler(int sig)
{
    return;
}

int main(int argc, char *argv[])
{
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    int s;

    request.tv_sec = getLong(argv[1], 0, "secs");
    request.tv_nsec = getLong(argv[2], 0, "nanosecs");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    if (gettimeofday(&start, NULL) == -1)
        errExit("gettimeofday");

    for (;;)
    {
        s = nanosleep(&request, &remain);
        if (s == -1 && errno != EINTR)
            errExit("nanosleep");

        if (gettimeofday(&finish, NULL) == -1)
            errExit("gettimeofday");

        printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec + 
                    (finish.tv_usec - start.tv_usec) / 1000000.0);

        if (s == 0)
            break;
        printf("Remaining: %2ld.%09ld\n", (long)remain.tv_sec, remain.tv_nsec);
        request = remain;
    }

    printf("Sleep complete\n");
    exit(EXIT_SUCCESS);
}