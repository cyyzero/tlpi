#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    printf("Hello world\n");
    write(STDOUT_FILENO, "Giao\n", 5);

    if (fork() == -1)
        errExit("fork");
    exit(EXIT_SUCCESS);
}