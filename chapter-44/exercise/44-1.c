#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int pfd1[2];
    int pfd2[2];
    char buf[1024];

    if (pipe(pfd1) == -1)
        errExit("pipe");
    if (pipe(pfd2) == -1)
        errExit("pipe");

    switch (fork())
    {
    case -1:
        errExit("fork");

    case 0:
        close(pfd1[1]);
        close(pfd2[0]);

        while (fgets(buf, 1024, stdin) != NULL)
        {
            size_t len = strlen(buf);
            if (write(pfd2[1], buf, len) != len)
            {
                errExit("write 1");
            }
            if (read(pfd1[0], buf, 1024) == -1)
                errExit("read 1");
            puts(buf);
        }
        close(pfd1[0]);
        close(pfd2[1]);
        break;

    default:
        close(pfd1[0]);
        close(pfd2[1]);
        for (;;)
        {
            ssize_t len = read(pfd2[0], buf, 1024);
            if (len == -1)
                errExit("read 2");
            if (len == 0)
                break;

            for (int i = 0; i < len; ++i)
            {
                buf[i] = toupper(buf[i]);
            }
            if (write(pfd1[1], buf, len) != len)
            {
                errExit("write 2");
            }
        }
        close(pfd1[1]);
        close(pfd2[0]);
        break;
    }
}