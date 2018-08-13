#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct stat sb;
    int fd, umask_bits;

    fd = open("test", O_CREAT | O_EXCL, 0777);

    if (fd == -1)
        errExit("open");

    if (stat("test", &sb) == -1)
        errExit("stat");

    umask_bits = ~(sb.st_mode & 0777) & 0777;

    printf("%04o\n", umask_bits);

    return 0;
}