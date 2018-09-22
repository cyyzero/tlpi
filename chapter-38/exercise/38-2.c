#define _BSD_SOURCE
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "tlpi_hdr.h"

uid_t check_password(const char *name);

int main(int argc, char *argv[])
{
    uid_t user_id;
    if (argc == 1 || argc == 2)
        fatal("Error args\n");
    if (strcmp("-u", argv[1]) == 0)
        user_id = check_password(argv[2]);
    else
        user_id = check_password("root");

    /**
     * WTF!!!!!
     * How can an unprivileged process change it's user-id?
     */

    if (setresuid(user_id, user_id, user_id) == -1)
        errExit("setresuid");

    if (execvp(argv[3], &argv[4]) == -1)
        errExit("execvp");
}

/**
 * Get password and check authority
 * if Ok, return user id, else exit
 */

uid_t check_user(const char *name)
{
    long name_len;
    char *password, *encrypted;
    int auth_ok;
    struct passwd *pwd;
    struct spwd *spwd;

    name_len = sysconf(_SC_LOGIN_NAME_MAX);
    if (name_len == -1)
        name_len = 256;

    if (strlen(name) > name_len)
        fatal("user name too long.\n");

    pwd = getpwnam(name);
    if (pwd == NULL)
        fatal("Couldn't get password record.\n");

    spwd = getspnam(name);
    if (spwd == NULL && errno == EACCES)
        fatal("No permission to read shadow password file\n");

    if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;

    password = getpass("Password: ");

    encrypted = crypt(password, pwd->pw_passwd);

    for (char* p = password; *p != '\0'; )
        *p++ = '\0';

    if (encrypted == NULL)
        errExit("crypt");

    auth_ok = strcmp(encrypted, pwd->pw_passwd) == 0;

    if (!auth_ok)
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);

    return pwd->pw_uid;
}