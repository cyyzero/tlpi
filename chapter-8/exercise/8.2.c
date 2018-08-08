#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>

struct passwd *mygetpwnam(const char*);

int main(int argc, char *argv[])
{
    struct passwd* passwd = mygetpwnam("cyyzero");
    if (passwd != NULL)
    {
        printf("%s\n%s\n%ld\n%ld\n%s\n%s\n%s", passwd->pw_name,
                                             passwd->pw_passwd,
                                             (long)passwd->pw_uid,
                                             (long)passwd->pw_gid,
                                             passwd->pw_gecos,
                                             passwd->pw_dir,
                                             passwd->pw_shell);
    }
}

struct passwd *mygetpwnam(const char* name)
{
    static char pw_name[21];
    static char pw_passwd[21];
    static char pw_gecos[101];
    static char pw_dir[21];
    static char pw_shell[21];
    static struct passwd ret_passwd = { pw_name, pw_passwd, 1000, 1000, 
                                        pw_gecos, pw_dir, pw_shell };

    struct passwd *passwd;

    while ((passwd = getpwent()) != NULL)
    {
        if (strcmp(name, passwd->pw_name) == 0)
        {
            strncpy(ret_passwd.pw_name, passwd->pw_name, 20);
            strncpy(ret_passwd.pw_passwd, passwd->pw_passwd, 20);
            ret_passwd.pw_uid = passwd->pw_uid;
            ret_passwd.pw_gid = passwd->pw_gid;
            strncpy(ret_passwd.pw_gecos, passwd->pw_gecos, 100);
            strncpy(ret_passwd.pw_dir, passwd->pw_dir, 20);
            strncat(ret_passwd.pw_shell, passwd->pw_shell, 20);
            return &ret_passwd;
        }
    }

    return NULL;
}