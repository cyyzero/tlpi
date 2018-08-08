#define _GNU_SOURCE
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>
#include "tlpi_hdr.h"
#include "../../chapter-8/example/ugid_functions.h"

#define SG_SIZE (NGROUPS_MAX+1)

int main(int argc, char *argv[])
{
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t supp_groups[SG_SIZE];
    int num_groups, j;
    char *p;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");
    if (getresgid(&rgid, &egid, &sgid) == -1)
        errExit("getresgid");

    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    printf("UID: ");
    p = user_name_from_id(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)ruid);
    p = user_name_from_id(euid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)euid);
    p = user_name_from_id(suid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)suid);
    p = user_name_from_id(fsuid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsuid);
    printf("\n");

    printf("GID: ");
    p = group_name_from_id(rgid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)rgid);
    p = group_name_from_id(egid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)egid);
    p = group_name_from_id(sgid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)sgid);
    p = group_name_from_id(fsgid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsgid);
    printf("\n");

    num_groups = getgroups(SG_SIZE, supp_groups);
    if (num_groups == -1)
        errExit("getgroups");

    printf("Supplementary groups (%d): ", num_groups);
    for (j = 0; j < num_groups; ++j)
    {
        p = group_name_from_id(supp_groups[j]);
        printf("%s (%ld)", (p == NULL) ? "???" : p, (long)supp_groups[j]);
    }
    printf("\n");
    exit(EXIT_SUCCESS);
}

