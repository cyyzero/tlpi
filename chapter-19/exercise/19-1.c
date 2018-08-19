#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <sys/inotify.h>
#include <limits.h>
#include "tlpi_hdr.h"

#define BUF_SIZE (10 * sizeof(struct inotify_event) + NAME_MAX + 1)

static int inotify_fd;

static void display_inotify_event(struct inotify_event *i)
{
    printf("    wd = %2d; ", i->wd);
    if (i->cookie > 0)
        printf("cookie =%4d; ", i->cookie);

    printf("mask = ");
    if (i->mask & IN_ACCESS)
        printf("IN_ACCESS");
    if (i->mask & IN_ATTRIB)
        printf("IN_ATTRIB");
    if (i->mask & IN_CLOSE_NOWRITE)
        printf("IN_CLOSE_NOWRITE");
    if (i->mask & IN_CLOSE_WRITE)
        printf("IN_CLOSE_WRITE");
    if (i->mask & IN_CREATE)
        printf("IN_CREATE");
    if (i->mask & IN_DELETE)
        printf("IN_DELETE");
    if (i->mask & IN_DELETE)
        printf("IN_DELETE");
    if (i->mask & IN_DELETE_SELF)
        printf("IN_DELETE_SELF");
    if (i->mask & IN_IGNORED)
        printf("IN_IGNORED");
    if (i->mask & IN_ISDIR)
        printf("IN_ISDIR");
    if (i->mask & IN_MODIFY)
        printf("IN_MODIFY");
    if (i->mask & IN_MOVE_SELF)
        printf("IN_MOVE_SELF");
    if (i->mask & IN_MOVED_FROM)
        printf("IN_MOVED_FROM");
    if (i->mask & IN_MOVED_TO)
        printf("IN_MOVED_TO");
    if (i->mask & IN_OPEN)
        printf("IN_OPEN");
    if (i->mask & IN_Q_OVERFLOW)
        printf("IN_Q_OVERFLOW");
    if (i->mask & IN_UNMOUNT)
        printf("IN_UNMOUNT");
    printf("\n");

    if (i->len > 0)
        printf("    name = %s\n", i->name);
}

static int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int wd;
    wd = inotify_add_watch(inotify_fd, fpath, IN_ALL_EVENTS);
    if (wd == -1)
        errExit("inotify_add_watch");

    printf("Watching %s using wd %d\n", fpath, wd);
    return 0;
}

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    ssize_t num_read;
    char *p;
    struct inotify_event *event;



    inotify_fd = inotify_init();
    if (inotify_fd == -1)
        errExit("Inotify_init");

    if (nftw(argv[1], fn, 10, 0) != 0)
        errExit("nftw");

    for (;;)
    {
        num_read = read(inotify_fd, buf, BUF_SIZE);
        if (num_read == 0)
            fatal("read() from inotify fd returned 0!");
        
        if (num_read == -1)
            errExit("read");

        printf("Read %ld bytes from inotify fd\n", (long)num_read);

        for (p = buf; p < buf + num_read; )
        {
            event = (struct inotify_event *)p;
            display_inotify_event(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    exit(EXIT_SUCCESS);
}
