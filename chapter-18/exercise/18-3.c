#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "tlpi_hdr.h"

char *my_realpath_helper(const char *path, char *resolved_path)
{
    if (path[0] == '/')
    {
        resolved_path[0] = '/';
        resolved_path[1] = '\0';
    }

    int i = 0, j = 0;
    while (path[i] != '\0')
    {
        while (path[i] == '/')
        {
            ++i;
        }
        j = i;
        while (path[j] != '/' && path[j] != '\0')
        {
            ++j;
        }
        if (j > i)
        {
            if (j - i == 1 && path[i] == '.')
            {
                // Do nothing.
            }
            else if (j-i == 2 && path[i] == '.' && path[i] == '.')
            {
                char *dirn = dirname(resolved_path);
                strncpy(resolved_path, dirn, PATH_MAX);
            }
            else
            {
                if (resolved_path[strlen(resolved_path)-1] != '/')
                {
                    strncat(resolved_path, "/", PATH_MAX);
                }
                size_t resolved_path_len = strlen(resolved_path);
                strncat(resolved_path, &path[i], j-i+1);
                struct stat stat_buf;
                if (lstat(resolved_path, &stat_buf) == -1)
                    return NULL;
                
                if (S_ISLNK(stat_buf.st_mode))
                {
                    char buf[PATH_MAX];
                    ssize_t len = readlink(resolved_path, buf, PATH_MAX);
                    if (len == -1)
                        return NULL;
                    resolved_path[resolved_path_len] = '\0';
                    if (my_realpath_helper(buf, resolved_path) == NULL)
                        return NULL;
                }
                //puts(resolved_path);
            }
        }
        i = j;
    }
    return resolved_path;
}

char *my_realpath(const char *path, char *resolved_path)
{
    if (path == NULL)
        return NULL;

    if (resolved_path == NULL)
        resolved_path = malloc(PATH_MAX);

    if (path[0] != '/')
    {
        if (getcwd(resolved_path, PATH_MAX) == NULL)
            return NULL;
        strncat(resolved_path, "/", PATH_MAX);
    }

    return my_realpath_helper(path, resolved_path);
}

int main(void)
{
    char *path = malloc(PATH_MAX);
    //printf("fuck\n");
    if (path == NULL)
        return 0;
    my_realpath("../../chapter-11/example", path);
    if (path)
        printf("%s\n", path);

    free(path);
    return 0;
}