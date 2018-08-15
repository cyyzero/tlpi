#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int fd, j, num_blocks;
    char shell_cmd[CMD_SIZE];
    char buf[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s temp-file [number-1kB-blocks] \n", argv[0]);

    num_blocks = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-1kB-blocks")
                            : 100000;

    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    system("ls -ahl");
    if (unlink(argv[1]) == -1)
        errExit("unlink");
    system("ls -ahl");
    for (j = 0; j < num_blocks; ++j)
    {
        if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
            fatal("partial/failed write");
    }

    snprintf(shell_cmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
    system(shell_cmd);

    if (close(fd) == -1)
        errExit("close");

    printf("******** Close file descriptor\n");
    system(shell_cmd);
    exit(EXIT_SUCCESS);
}