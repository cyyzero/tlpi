#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    execlp(argv[1], argv[1], "Hello world", NULL);
    errExit("execlp");
}