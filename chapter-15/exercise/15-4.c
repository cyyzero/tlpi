#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

int effective_access(const char *name, int mode)
{
    int ret;
    uid_t euid, ruid;
    gid_t egid, rgid;

    euid = geteuid();
    ruid = getuid();
    setresuid(euid, ruid, -1);
    rgid = getgid();
    egid = getegid();
    setresgid(egid, rgid, -1);

    ret = access(name, mode);

    setresuid(ruid, euid, -1);
    setresgid(rgid, egid, -1);

    return ret;
}

int main(int argc, char argv[])
{
    printf("%d\n", effective_access("15-4.c", F_OK | R_OK | W_OK | X_OK));
    return 0;
}