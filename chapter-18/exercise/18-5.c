#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "tlpi_hdr.h"

char *my_getcwd_helper(char *buf, size_t size, size_t *i)
{
    DIR *parent_dir;

    if ((parent_dir = opendir("..")) == NULL)
    {
        goto close_and_return;
    }
    else
    {
        struct dirent *dir;
        struct stat sb1, sb2;
        char b[256+3];
        char *ret;
        size_t len;

        if (stat(".", &sb1) == -1)
            goto close_and_return;

        while ((dir = readdir(parent_dir)) != NULL)
        {
            //printf("%s ", dir->d_name);
            sprintf(b, "../%s", dir->d_name);
            if (stat(b, &sb2) == -1)
                goto close_and_return;
            if (sb1.st_ino == sb2.st_ino && sb1.st_dev == sb2.st_dev)
            {
                break;
            }
        }
        puts(dir->d_name);
    printf("\n");

        len = strlen(dir->d_name);
        *i += (len+1);
        if (*i + 1 > size)
            goto close_and_return;
        sprintf(b, "%s/", dir->d_name);
        //puts(b);
        if (strcmp(dir->d_name, "..") == 0)
        {
            strcpy(buf, "/");
            ++(*i);
            return buf;
        }
        else
        {
            chdir("..");
            ret = my_getcwd_helper(buf, size, i);
            //printf("%s ", dir->d_name);
            strcat(buf, b);
            closedir(parent_dir);
            return ret;
        }

    }

close_and_return:
    closedir(parent_dir);
    return NULL;
}

char *my_getcwd(char *buf, size_t size)
{
    if (size == 0)
        return NULL;
    int fd;
    char *ret;
    size_t i = 0;

    fd = open("./", O_RDONLY);
    ret =  my_getcwd_helper(buf, size, &i);
    fchdir(fd);

    return ret;
}
int main(void)
{
    char buf[1024];

    if (my_getcwd(buf, 1024) == buf)
        puts(buf);

    return 0;
}