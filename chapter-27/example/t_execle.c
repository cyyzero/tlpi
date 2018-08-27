#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *env_vec[] = {"GREET=salut", "BYE=adieu", NULL};
    char *filename;

    filename = strchr(argv[1], '/');
    if (filename != NULL)
        filename++;
    else
        filename = argv[1];

    execle(argv[1], filename, "hello world", NULL, env_vec);
    errExit("execle");
}