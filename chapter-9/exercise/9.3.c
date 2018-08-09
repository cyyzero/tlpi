
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N (NGROUPS_MAX+1)

int myinitgroups(const char *user, gid_t group);

int main(void)
{
    setuid(0);
    if (myinitgroups("cyyzero", 42) == -1)
    {
        printf("initgroups error\n");
        exit(-1);
    }
    
    gid_t groups[N];
    int group_num;
    if ((group_num = getgroups(N, groups)) == -1)
        printf("getgroups error\n");

    printf("Groups: ");
    for (int i = 0; i < group_num; ++i)
    {
        printf("%ld ", (long)groups[i]);
    }
    printf("\n");
    return 0;
    

}

int myinitgroups(const char *user, gid_t group)
{
    size_t i = 0;
    gid_t groups[N];

    struct group *grp;
    while ((grp = getgrent()) != NULL)
    {
        char **mem = grp->gr_mem;
        while (*mem != NULL)
        {
            if (strcmp(user, *mem) == 0)
            {
                groups[i++] = grp->gr_gid;
                break;
            }
            ++mem;
        }
    }
    endgrent();
    groups[i++] = group;
    return setgroups(i, groups);
}