#define _BSD_SOURCE
#define _XOPEN_SOURCE
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *user_name, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    Boolean authOk;
    size_t len;
    long lnmax;
    lnmax = sysconf(_SC_LOGIN_NAME_MAX);

    if (lnmax == -1)
        lnmax = 256;

    user_name = malloc(lnmax);

    if (user_name == NULL)
    {
        errExit("malloc");
    }

    printf("Username: ");
    fflush(stdout);
    if (fgets(user_name, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE);
    
    len = strlen(user_name);
    if (user_name[len-1] == '\n')
        user_name[len-1] = '\0';

    pwd = getpwnam(user_name);
    if (pwd == NULL)
        fatal("couldn't get password record");

    spwd = getspnam(user_name);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");

    if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;
    
    
    password = getpass("Password: ");

    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; )
        *p++ = '\0';
    
    if (encrypted == NULL)
        errExit("crypt");

    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOk)
    {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    printf("Successfully authenticated : UID=%ld\n", (long)pwd->pw_uid);

    exit(EXIT_SUCCESS);


}