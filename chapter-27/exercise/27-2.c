#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
extern char **environ;

int execle(const char *path, const char *arg, ...
           /*, (char *)NULL, char * const envp[]*/)
{
    const char * args_list[200];
    char pathname[200];
    bool has_slash;
    size_t i;
    va_list args;
    va_start(args, arg);
    char *const *envp;

    has_slash = false;

    i = 0;
    args_list[i++] = arg;
    for (const char *p; (p = va_arg(args, const char*)) != NULL; )
    {
        args_list[i++] = p;
        printf("%s", p);
    }

    args_list[i] = NULL;

    envp = va_arg(args, char * const *);
    assert(environ == envp);
    va_end(args);

    for (int i = 0; path[i] != '\0'; ++i)
    {
        if (path[i] == '/')
        {
            has_slash = true;
            break;
        }
    }

    if (has_slash)
    {
        execve(path, (char * const*)args_list, envp);
    }
    else
    {
        char *env_path = getenv("PATH");
        const char *end, *begin;

        begin = end = env_path;
        while ((end = strchr(end, ':')) != NULL)
        {
            size_t len = end - begin;

            memcpy(pathname, begin, len+1);
            pathname[len] = '/';
            pathname[len+1] = '\0';
            strncat(pathname, path, 199-len);
            //args_list[0] = pathname;
            execve(pathname, (char * const*)args_list, envp);
            end++;
            begin = end;
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    execle("ls", "ls",  "-l", "-a", "-h", NULL, environ);
}