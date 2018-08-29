#define _BSD_SOURCE
#include <unistd.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (acct(argv[1]) == -1)
        errExit("acct");

    printf("Process accounting %s\n", (argv[1] == NULL) ? "disabled" : "enabled");
    exit(EXIT_SUCCESS);
}