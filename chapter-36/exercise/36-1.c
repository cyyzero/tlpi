#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdio.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int status;
    struct rusage sr;

    switch (fork())
    {
    case -1:
        errExit("fork");
    case 0:
        for (int i = 0; i < 1000000000; ++i)
            continue;
        _exit(EXIT_SUCCESS);
    default:
        if (getrusage(RUSAGE_CHILDREN, &sr) == -1)
            errExit("getrusage");

        printf("%ld\t%ld\n", sr.ru_utime.tv_sec, sr.ru_utime.tv_usec);
        if (wait(&status) == -1)
            errExit("wait");
        //printf("%d\n", status);
        if (getrusage(RUSAGE_CHILDREN, &sr) == -1)
            errExit("getrusage");
        printf("%ld\t%ld\n", sr.ru_utime.tv_sec, sr.ru_utime.tv_usec);
    }
}