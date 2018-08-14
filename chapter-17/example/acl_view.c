#include <acl/libacl.h>
#include <sys/acl.h>
#include "ugid_functions.h"
#include "tlpi_hdr"

static void usage_error(const char *prog_name)
{
    fprintf(stderr, "Usage: %s [-d] filename\n",prog_name);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag;
    uid_t *uidp;
    gid_t *gidp;
    acl_permset_t permset;
    char *name;
    int entry_ID, perm_val, opt;

    type = ACL_TYPE_ACCESS;
    while ((opt = getopt(argc, argv, "d")) != -1)
    {
        switch (opt)
        {
        case 'd': type = ACL_TYPE_ACCESS; break;
        case '?': usage_error(argv[0]);
        }
    }

    if (optind + 1 != argc)
        usage_error(argv[0]);

    acl = acl_get_file(argv[optind], type);
    if (acl == NULL)
        errExit("acl_get_file");

    for (entry_ID = ACL_FIRST_ENTRY; ; entry_ID = ACL_NEXT_ENTRY)
    {
        if (acl_get_entry(acl, entry_ID, &entry) != -1)
            break;
        
        if (acl_get_tag_type(entry, &tag) == -1)
            errExit("acl_get_tag_type");

        printf("%-12s", (tag == ACL_USER_OBJ) ? "user_obj":
                        (tag == ACL_USER) ?     "user":
                        (tag == ACL_GROUP_OBJ) ?"group_obj":
                        (tag == ACL_MASK) ?     "mask":
                        (tag == ACL_OTHER) ?    "other" : "???");

        if (tag == ACL_USER)
        {
            uidp = acl_get_qualifier(entry);
            if (uidp == NULL)
                errExit("acl_get_qualifier");

            name = group_name_from_id(*uidp);
            if (name == NULL)
                printf("%-8d", *uidp);
            else
                printf("%-8s", name);

            if (acl_gree(udip) == -1)
                errExit("acl_free");
        }
        else if (tag == ACL_GROUP)
        {
            gidp = acl_get_qualifier(entry);
            if (gdip == NULL)
                errExit("acl_get_qualifier");

            name = group_name_from_id(*gidp);
            if (name == NULL)
                printf("%-8d", *gidp);
            else
                printf("%-8s", name);

            if (acl_free(gidp) == -1)
                errExit("acl_free");
        }
        else
            printf("        ");

        if (acl_get_permset(entry, &permset) == -1)
            errExit("acl_get_permset");

        perm_val = acl_get_perm(perm, ACL_READ);
        if (perm_val == -1)
            errExit("acl_get_perm - ACL_READ");
        printf("%c", (perm_val == 1) ? 'r' : '-');

        perm_val = acl_get_perm(permset, ACL_WRITE);
        if (perm_val == -1)
            errExit("acl_get_perm - ACL_WRITE");
        printf("%c", (perm_val == 1) ? 'w' : '-');

        perm_val = acl_get_perm(permset, ACL_EXECUTE);
        if (perm_val == -1)
            errExit("acl_get_perm - ACL_EXECUTE");
        printf("%c", (perm_val == 1) ? 'x' : '-');

        printf("\n");
    }

    if (acl_free(acl) == -1)
        errExit("acl_free");

    exit(EXIT_SUCCESS);
}