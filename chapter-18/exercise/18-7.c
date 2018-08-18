#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <string.h>

size_t sock, lnk, reg, blk, dir, chr, fifo;

int fn(const char *fpath, const struct stat *sb,
       int typeflag, struct FTW *ftwbuf)
{
    for (int i = 0; i < ftwbuf->level; ++i)
        printf("\t");

    printf("%s\n", &fpath[ftwbuf->base]);
    switch (sb->st_mode & S_IFMT)
    {
    case S_IFSOCK: ++sock; break;
    case S_IFLNK:  ++lnk;  break;
    case S_IFREG:  ++reg;  break;
    case S_IFBLK:  ++blk;  break;
    case S_IFDIR:  ++dir;  break;
    case S_IFCHR:  ++chr;  break;
    case S_IFIFO:  ++fifo; break;
    }
    return 0;
}

int main(void)
{
    nftw("../../", fn, 10, 0);
    printf("%ld %ld %ld %ld %ld %ld %ld\n", sock, lnk, reg, blk, dir, chr, fifo);
    return 0;
}