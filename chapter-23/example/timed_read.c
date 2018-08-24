#define _GNU_SOURCE
#include <signal.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 200

static void handler(int sig)
{
    printf("Caught signal\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    char buf[BUF_SIZE];
    ssize_t num_read;
    int saved_errno;

    sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

    alarm((argc > 1) ? getInt(argv[1], GN_NONNEG, "num-secs") : 10);

    num_read = read(STDIN_FILENO, buf, BUF_SIZE - 1);

    saved_errno = errno;
    alarm(0);
    
    if (num_read == -1)
    {
        if (saved_errno == EINTR)
            printf("Read timed out\n");
        else
            errMsg("read");
    }
    else
    {
        printf("Successful read (%ld bytes): %.*s", (long)num_read, (int)num_read, buf);
    }
    exit(EXIT_SUCCESS);
}