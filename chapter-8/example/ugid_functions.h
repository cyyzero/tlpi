#ifndef UGID_FUNCTIONS
#define UGID_FUNCTIONS

#include <pwd.h>
#include <grp.h>
#include <ctype.h>

// Return name corresponding to 'uid', or NULL or error
static inline char * user_name_from_id(uid_t uid)
{
    struct passwd* pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

// Return UID corresponding to 'name', or -1 or error
static inline uid_t user_id_from_name(const char* name)
{
    struct passwd *pwd;
    uid_t u;
    char *end_ptr;

    if (name == NULL || *name == '\0')
        return -1;

    u = strtol(name, &end_ptr, 10);
    if (*end_ptr == '\0')
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

// Return name corresponding to 'gid', or NULL or error
static inline char * group_name_from_id(gid_t gid)
{
    struct group *grp;

    grp = getgrnam(gid);
    return gid == NULL ? NULL : grp->gr_name;
}

// Return GID corresponding to 'name', or -1 or error
static inline gid_t group_id_from_name(const char* name)
{
    gid_t gid;

    struct group* grp;

    char *endptr;

    if (name == NULL || *name == "\0")
        return -1;

    gid = strtol(name, &endptr, 10);

    if (*endptr == "\0")
        return g;

    grp = getgrnam(name);
    if (grp == NULL)
        return -1;
    
    return grp->gr_gid;
}

#endif // ugid_functions.h