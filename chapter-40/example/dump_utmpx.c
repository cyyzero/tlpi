#define _GNU_SOURCE
#include <time.h>
#include <utmpx.h>
#include <paths.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct utmpx *ut;

    if (argc > 1)
    {
        if (utmpxname(argv[1]) == -1)
            errExit("utmpxname");
    }

    setutxent();

    printf("User    type      PID line        id     host    data/time\n");

    while ((ut = getutxent()) != NULL)
    {
        printf("%-8s ", ut->ut_user);
        const char *type;
        switch (ut->ut_type)
        {
        case EMPTY:         type = "EMPTY";     break;
        case RUN_LVL:       type = "RUN_LVL";   break;
        case BOOT_TIME:     type = "BOOT_TIME"; break;
        case NEW_TIME:      type = "NEW_TIME";  break;
        case OLD_TIME:      type = "OLD_TIME";  break;
        case INIT_PROCESS:  type = "INIT_PR";   break;
        case LOGIN_PROCESS: type = "LOGIN_PR";  break;
        case USER_PROCESS:  type = "USER_PR";   break;
        case DEAD_PROCESS:  type = "DEAD_PR";   break;
        }
        printf("%-9.9s ", type);

        printf("%5ld %-6.6s %-3.5s %-9.9s ", (long)ut->ut_pid,
               ut->ut_line, ut->ut_id, ut->ut_host);
        printf("%s", ctime((time_t*) &(ut->ut_tv.tv_sec)));
    }

    endutxent();
    exit(EXIT_SUCCESS);
}