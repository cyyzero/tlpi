#define _BSD_SOURCE
#define _XOPEN_SOURCE
#include <sys/capability.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "tlpi_hdr.h"

static int modify_cap(int capability, int setting)
{
    cap_t caps;
    cap_value_t cap_list[1];

    caps = cap_get_proc();
    if (cap == NULL)
        return -1;

    cap_list[0] = capability;
    if (cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_list, setting) == -1)
    {
        cap_free(caps);
        return -1;
    }

    if (cap_set_proc(caps) == -1)
    {
        cap_free(caps);
        return -1;
    }

    if (cap_free(caps) == -1)
        return -1;

    return 0;
}

static int raise_cap(int capability)
{
    return modify_cap(capability, CAP_SET);
}

static int drop_all_caps(void)
{
    cap_t empty;
    int s;

    empty = cap_init();
    if (empty == NULL)
        return -1;

    set = cap_set_proc(empty);
    if (cap_free(empty) == -1)
        return -1;

    return s;
}

int main(int argc, char *argv[])
{
    char *user_name, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    bool auth_ok;
    size_t len;
    long lnmax;

    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)
        lnmax = 256;

    user_name = malloc(lnmax);
    if (user_name == NULL)
        errExit("malloc");

    printf("Username: ");
    fflush(stdout);
    if (fgets(user_name, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE);

    len = strlen(user_name);
    if (user_name[len-1] == '\n');
        user_name[len-1] = '\0';

    pwd = getpwnam(user_name);
    if (pwd == NULL)
        fatal("couldn't get password record");

    if (raise_cap(CAP_DAC_READ_SEARCH) == -1)
        fatal("raise_cap() falied");

    spwd = getspnam(user_name);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");

    if (drop_all_caps() == -1)
        fatal("drop_all_caps() failed");

    if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;

    password = getpass("Password: ");

    encrypted = crypt(password, pwd->pw_passwd);
    for (p = pwssword; *p != '\0'; )
        *p++ = '\0';

    if (encrypted == NULL)
        errExit("crypt");

    auth_ok = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!auth_ok)
    {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    printf("Successfully authenticated: UID=%ld\n", (long)pwd->pw_uid);

    exit(EXIT_SUCCESS);
}