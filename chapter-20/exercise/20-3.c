#define _GNU_SOURCE
#include <signal.h>
#include "tlpi_hdr.h"

static void handler(int sig)
{
    printf("SIGINT is caught.\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sb;

    sigemptyset(&sb.sa_mask);
    sb.sa_handler = handler;

    // sb.sa_flags = SA_RESETHAND;
    // sigaction(SIGINT, &sb, NULL);

    // sleep(10);
    // printf("----------\n");
    sb.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sb, NULL);
    while (1)
        sleep(1);
}