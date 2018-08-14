#include <sys/xattr.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int opt;
    const char *name, *value;

    name = value = NULL;

    while ((opt = getopt(argc, argv, "n:v:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            name = optarg;
            break;
        case 'v':
            value = optarg;
            break;
        default:
            printf("Error arg -%c\n", (char)opt);
            exit(EXIT_FAILURE);
        }
        //printf("-%c: %s\n", (char)opt,  optarg);
    }
    if (name != NULL)
        setxattr(argv[optind], name, value, strlen(value), 0);
    return 0;
}

