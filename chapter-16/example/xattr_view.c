#include <sys/xattr.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

#define XATTR_SIZE 10000

static void usage_error(const char *prog_name)
{
    fprintf(stderr, "Usage: %s [-x] file...\n", prog_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    char list[XATTR_SIZE], value[XATTR_SIZE];
    ssize_t list_len, value_len;
    int ns, j, k, opt;
    bool hex_display;

    hex_display = false;
    while ((opt = getopt(argc, argv, "x")) != -1)
    {
        switch (opt)
        {
        case 'x':
            hex_display = 1;
            break;
        case '?':
            usage_error(argv[0]);
        }
    }

    if (optind >= argc + 2)
        usage_error(argv[0]);

    for (j = optind; j < argc; ++j)
    {
        list_len = listxattr(argv[j], list, XATTR_SIZE);
        if (list_len == -1)
            errExit("listxattr");
        
        printf("%s:\n", argv[j]);

        for (ns = 0; ns < list_len; ns += strlen(&list[ns]) + 1)
        {
            printf("    name=%s;", &list[ns]);

            value_len = getxattr(argv[j], &list[ns], value, XATTR_SIZE);
            if (value_len == -1)
                printf("couldn't get value");
            else if (!hex_display)
                printf("value=%.*s", (int)value_len, value);
            else {

                printf("value=");
                for (k = 0; k < value_len; ++k)
                    printf("%02x", (unsigned int)value[k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    exit(EXIT_SUCCESS);
}