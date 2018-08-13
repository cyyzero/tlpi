#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "tlpi_hdr.h"

#define MYFILE "myfile"
#define MYDIR  "mydir"
#define FILE_PERMS     (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS      (S_IRWXU | S_IRWXG | S_IRWXO)
#define UMASK_SETTING  (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)
#define FP_SPECIAL 1
#define STR_SIZE sizeof("rwxrwxrwx")

char *file_perm_str(mode_t perm, int flags)
{
    static char str[STR_SIZE];
    snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
             (perm & S_IRUSR) ? 'r' : '-',
             (perm & S_IWUSR) ? 'w' : '-',
             (perm & S_IXUSR) ? (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
                                (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
             (perm & S_IRGRP) ? 'r' : '-',
             (perm & S_IWGRP) ? 'w' : '-',
             (perm & S_IXGRP) ? (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
                                (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
             (perm & S_IROTH) ? 'r' : '-',
             (perm & S_IWOTH) ? 'w' : '-',
             (perm & S_IXOTH) ? (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
                                (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));

    return str;
}

int main(int argc, char *argv[])
{
    int fd;
    struct stat sb;
    mode_t u;

    umask(UMASK_SETTING);

    fd = open(MYFILE, O_CREAT | O_EXCL, FILE_PERMS);
    if (fd == -1)
        errExit("open");
    if (mkdir(MYDIR, DIR_PERMS) == -1)
        errExit("mkdir-%s", MYDIR);
    
    u = umask(0);

    if (stat(MYFILE, &sb) == -1)
        errExit("stat-%s", MYFILE);
    printf("Requested file perms: %s\n", file_perm_str(FILE_PERMS, 0));
    printf("process umask:        %s\n", file_perm_str(u, 0));
    printf("Actual file perms:    %s\n", file_perm_str(sb.st_mode, 0));
    if (stat(MYDIR, &sb) == -1)
        errExit("stat-%s", MYDIR);
    printf("Requested dir. perms: %s\n", file_perm_str(DIR_PERMS, 0));
    printf("Process umask:        %s\n", file_perm_str(u, 0));
    printf("Actual dir. perms:    %s\n", file_perm_str(sb.st_mode, 0));

    if (unlink(MYFILE) == -1)
        errMsg("unlink-%s", MYFILE);
    if (rmdir(MYDIR) == -1)
        errMsg("rmdir-%s", MYDIR);
    exit(EXIT_SUCCESS);
}

