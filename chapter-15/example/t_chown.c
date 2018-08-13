#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include "ugid_functions.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    uid_t uid;
    gid_t gid;
    int j;
    Boolean err_fnd;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s owner group [file...]\n"
                 "       owner or group can be '-',"
                 "meaning leave unchanged\n", argv[1]);
    }

    if (strcmp(argv[1], "-") == 0)
        uid = -1;
    else
    {
        uid = user_id_from_name(argv[1]);
        if (uid == -1)
            fatal("No such user (%s)", argv[1]);
    }

    if (strcmp(argv[2], "-") == 0)
        gid = -1;
    else
    {
        gid = group_id_from_name(argv[2]);
        if (gid == -1)
            fatal("No group user (%s)", argv[1]);
    }

    err_fnd = FALSE;
    for (j = 3; j < argc; ++j)
    {
        if (chown(argv[j], uid, gid) == -1)
        {
            errMsg("chown: %s", argv[j]);
            err_fnd = TRUE;
        }
    }

    exit(err_fnd ? EXIT_FAILURE : EXIT_SUCCESS);
}