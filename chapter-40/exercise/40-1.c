#include <utmp.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <stdio.h>

char *my_getlogin(void)
{
    static char buf[UT_NAMESIZE];
    char *tty_name;
    struct utmp *line;

    if ((tty_name = ttyname(STDIN_FILENO)) == NULL)
    {
        return NULL;
    }

    tty_name = basename(tty_name);

    setutent();

    //puts(tty_name);
    while ((line = getutent()) != NULL)
    {
        //puts(line->ut_line);
        if (strcmp(line->ut_line, tty_name) == 0)
        {
            strcpy(buf, line->ut_user);
            return buf;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    char *p = my_getlogin();
    puts(p ? p : "fuck");
}
