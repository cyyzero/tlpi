#include "tlpi_hdr.h"

extern char **environ;

int main(int argc, char *argv[])
{
    int j;
    char **ep;

    for (j = 0; j < argc; ++j)
        printf("argv[%d] = %s\n", j, argv[j]);

    for (ep = environ; *ep != NULL; ++ep)
        printf("envirn: %s\n", *ep);

    exit(EXIT_SUCCESS);
}