#include <sys/time.h>
#include <sys/resource.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int which, prio;
    id_t who;

    which = (argv[1][0] == 'p') ? PRIO_PROCESS :
                 (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;

    who = getLong(argv[2], O, "who");
    prio = getInt(argv[3], O, "prio");

    if (setpriority(which, who, prio) == -1)
        errExit("getpriority");

    errno = 0;
    prio = getpriority(which, who);
    if (prio == -1 && errno != 0)
        errExit("getpriority");

    printf("Nice value = %d\n", prio);

    exit(EXIT_SUCCESS);
}