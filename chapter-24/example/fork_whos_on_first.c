#include <sys/wait.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int num_children, j;
    pid_t child_pid;

    num_children = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-children") : 1;

    setbuf(stdout, NULL);

    for (j = 0; j < num_children; ++j)
    {
        switch (child_pid = fork())
        {
        case -1:
            errExit("fork");
        case 0:
            printf("%d child\n", j);
            _exit(EXIT_SUCCESS);
        default:
            printf("%d parent\n", j);
            wait(NULL);
            break;
        }
    }
    exit(EXIT_SUCCESS);
}