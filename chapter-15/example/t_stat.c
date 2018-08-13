#define _BSD_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "tlpi_hdr.h"

static void display_stat_info(const struct stat* sb)
{
    printf("File type:          ");
    switch (sb->st_mode & S_IFMT)
    {
    case S_IFREG:  printf("regular files\n");        break;
    case S_IFDIR:  printf("directory\n");            break;
    case S_IFCHR:  printf("character device\n");     break;
    case S_IFBLK:  printf("block device\n");         break;
    case S_IFLNK:  printf("symbolic (soft) link\n"); break;
    case S_IFIFO:  printf("FIFO or pipe\n");         break;
    case S_IFSOCK: printf("socket");                 break;
    default:       printf("unkown file type?\n");    break;
    }

    printf("Device containing i-node: major=%ld   minor=%ld\n",
           (long)major(sb->st_dev), (long)minor(sb->st_dev));

    printf("I-node number:      %ld\n", (long)sb->st_ino);

    // printf("Mode:               %lo (%s)\n",
    //        (unsigned long)sb->st_mode, file_perm_str(sb->st_mode, 0));

    if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
        printf("    special bits set:    %s%s%s\n",
               (sb->st_mode & S_ISUID) ? "set-UID" : "",
               (sb->st_mode & S_ISGID) ? "set-GID" : "",
               (sb->st_mode & S_ISVTX) ? "sticky"  : "");
    
    printf("Ownership:        UID=%ld   GID=%ld\n",
           (long)sb->st_uid, (long)sb->st_gid);
    
    if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode));
        printf("Device number (st_rdev): major=%ld; minor=%ld\n",
               (long)major(sb->st_rdev), (long)minor(sb->st_rdev));

    printf("File size:              %lld bytes\n", (long long)sb->st_size);
    printf("Optimal I/O block size: %ld bytes\n", (long)sb->st_blksize);
    printf("512B block allocated:   %lld\n", (long long)sb->st_blocks);
    printf("Last file access:       %s", ctime(&sb->st_atime));
    printf("Last file modification: %s", ctime(&sb->st_mtime));
}

int main(int argc, char *argv[])
{
    struct stat sb;
    Boolean stat_link;
    int fname;

    stat_link = (argc > 1) && strcmp(argv[1], "-1") == 0;

    fname = stat_link ? 2 : 1;

    if (stat_link)
    {
        if (lstat(argv[fname], &sb) == -1)
            errExit("lstat");
    }
    else
    {
        if (stat(argv[fname], &sb) == -1)
            errExit("stat");
    }

    display_stat_info(&sb);

    exit(EXIT_SUCCESS);
}