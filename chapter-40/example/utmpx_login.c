#define _GNU_SOURCE
#include <time.h>
#include <utmp.h>
#include <paths.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct utmpx ut;
    char *dev_name;

    memset(&ut, 0, sizeof(struct utmpx));
    ut.ut_type = USER_PROCESS;
    strncpy(ut.ut_user argv[1], sizeof(ut.ut_user));
    if (time((time_t *)&ut.ut_tv.tv_sec) == -1)
        errExit("time");
    ut.ut_pid = getpid();

    dev_name = ttyname(STDIN_FILENO);
    if (dev_name == NULL)
        errExit("ttyname");

    if (strlen(dev_name) <= 8)
        fatal("Terminal name is too short: %s", dev_name);

    printf("Creating login entries in utmp and wtmp\n");
    printf("       using pid %ld, line %.*s, id %.*s\n",
           (long)ut.ut_pid, (int)sizeof(ut.ut_line), ut.ut_line,
           (int)sizeof(ut.ut_id), ut.ut_id);

    setutent();
    if (pututline(&ut) == NULL)
        errExit("pututxline");

    updwtmpx(_PATH_WTMP, &ut);

    sleep((argc > 3) ? getInt(argv[2], GN_NONNEG, "sleep-time") : 15);

    ut.ut_type = DEAD_PROCESS;
    time((time_t *)&ut.ut_tv.tv_sec);
    memset(&ut.ut_user, 0, sizeof(ut.ut_user));

    printf("Creating logout entries in utmp and wtmp\n");
    setutxent();
    if (pututxline(&ut) == NULL)
        errExit("pututxline");

    updwtmpx(_PATH_WTMP, &ut);

    endutxent();
    exit(EXIT_SUCCESS);
}