#include <dirent.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

static void list_files(const char *dirpath)
{
    DIR *dirp;
    struct dirent *dp;
    bool is_current;

    is_current = strcmp(dirpath, ".") == 0;

    dirp = opendir(dirpath);
    if (dirp == NULL)
    {
        errMsg("opendir failed on '%s'", dirpath);
        return;
    }
    //printf("fuck");

    for (;;)
    {
        errno = 0;
        dp = readdir(dirp);
        if (dp == NULL)
            break;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        if (!is_current)
            printf("%s/", dirpath);
        printf("%s\n", dp->d_name);
    }

    if (errno != 0)
        errExit("readdir");
    if (closedir(dirp) == -1)
        errMsg("closedir");
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [dir...]\n", argv[0]);

    if (argc == 1)
        list_files(".");
    else
        for (argv++; *argv; ++argv)
            list_files(*argv);

    exit(EXIT_SUCCESS);
}