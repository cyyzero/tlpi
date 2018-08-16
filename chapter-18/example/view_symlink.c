#include <sys/stat.h>
#include <limits.h>
#include "tlpi_hdr.h"

#define BUF_SIZE PATH_MAX

int main(int argc, char *argv[])
{
    struct stat statbuf;
    char buf[BUF_SIZE];
    ssize_t num_bytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);
    if (lstat(argv[1], &statbuf) == -1)
        errExit("lstat");

    if (!S_ISLNK(statbuf.st_mode))
        fatal("%s is not a symbolic link", argv[1]);

    num_bytes = readlink(argv[1], buf, BUF_SIZE-1);
    if (num_bytes == -1)
        errExit("readlink");

    buf[num_bytes] = '\0';
    printf("readlink: %s --> %s\n", argv[1], buf);

    exit(EXIT_SUCCESS);
}