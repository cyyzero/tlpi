#include <utmpx.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct utmpx *ut;
    setutxent();

    while ((ut = getutxent()) != NULL)
    {
        if (ut->ut_type == USER_PROCESS || ut->ut_type == LOGIN_PROCESS)
        {
            printf("%-8s ", ut->ut_user);
            printf("%-5s ", ut->ut_line);

            printf("%s", ctime((time_t*) &(ut->ut_tv.tv_sec)));
        }
    }
}