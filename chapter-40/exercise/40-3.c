#include <utmp.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void my_login(const struct utmp *ut)
{
    // char *dev_name;

    // memset(&ut, 0, sizeof(struct utmp));

    // ut->ut_type = USER_PROCESS;
    // ut->ut_pid = getpid();
    // time((time_t *)&ut->ut_tv.tv_sec);
    // dev_name = ttyname(STDIN_FILENO);
    // if (dev_name == NULL)
    // {
    //     dev_name = ttyname(STDOUT_FILENO);
    //     if (dev_name == NULL)
    //     {
    //         dev_name = ttyname(STDERR_FILENO);
    //         if (dev_name == NULL)
    //         {
    //             strncpy(ut->ut_line, "???", sizeof(UT_LINESIZE));
    //         }
    //     }
    // }

    // strncpy(ut->ut_line, dev_name + 5, sizeof(UT_LINESIZE));
    // strncpy(ut->ut_id, dev_name+8, 4);

    setutent();
    pututline(ut);
    updwtmp(_PATH_WTMP, ut);
}

int my_logout(const struct utmp *ut_line)
{
    struct utmp *ut;

    setutent();
    while ((ut = getutent()) != NULL)
    {
        if (strcmp(ut_line->ut_line, ut->ut_line) && ut_line->ut_id == ut->ut_id)
        {
            ut->ut_type = DEAD_PROCESS;
            memset(ut->ut_user, 0, UT_NAMESIZE);
            memset(ut->ut_host, 0, UT_HOSTSIZE);
            time((time_t *)&ut->ut_tv.tv_sec);
            return 1;
        }
    }
    endutent();
    return 0;
}

void my_logwtmp(const char *line, const char *name, const char *host)
{
    struct utmp ut;

    memset(&ut, 0, sizeof(ut));
    if (strlen(line) + 1 > UT_LINESIZE)
        return;

    strcpy(ut.ut_line, line);

    if (strlen(name) + 1 > UT_NAMESIZE)
        return;

    strcpy(ut.ut_user, name);

    if (strlen(host) + 1 > UT_HOSTSIZE)
        return;
    strcpy(ut.ut_host, host);

    updwtmp(_PATH_WTMP, &ut);
}