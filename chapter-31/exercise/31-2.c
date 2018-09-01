#include <pthread.h>
#include "tlpi_hdr.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static pthread_once_t dirname_once = PTHREAD_ONCE_INIT;
static pthread_once_t basename_once = PTHREAD_ONCE_INIT;
static pthread_key_t dirname_key;
static pthread_key_t basename_key;

static void destory(void *buf)
{
    free(buf);
}

static void dirname_create_key(void)
{
    int s;

    s = pthread_key_create(&dirname_key, destory);
    if (s != 0)
        errExitEN(s, "pthread_key_create");
}

char *dirname(char *path)
{
    int s;
    char *buf;
    char *slash;

    s = pthread_once(&dirname_once, dirname_create_key);

    buf = pthread_getspecific(dirname_key);
    if (buf == NULL)
    {
        buf = malloc(PATH_MAX);
        if (buf == NULL)
            errExit("malloc");

        s = pthread_setspecific(dirname_key, buf);
        if (s != 0)
            errExitEN(s, "pthread_setspecific");
    }

    slash = NULL;
    for (char *cur = path; *cur != '\0'; ++cur)
    {
        if (*cur == '/')
            slash = cur;
    }

    if (slash)
    {
        size_t len = slash-path;
        len = (PATH_MAX-1 > len) ? len : PATH_MAX - 1;
        strncpy(buf, path, len);
        buf[len] = '\0';
    }
    else
    {
        buf[0] = '.';
        buf[1] = '\0';
    }
    return buf;
}

static void basename_create_key(void)
{
    int s;

    s = pthread_key_create(&basename_key, destory);
    if (s != 0)
        errExitEN(s, "pthread_key_create");
}

char *basename(char *path)
{
    int s;
    char *buf;

    s = pthread_once(&basename_once, basename_create_key);
    if (s != 0)
        errExitEN(s, "pthread_once");

    buf = pthread_getspecific(basename_key);
    if (buf == NULL)
    {
        buf = malloc(PATH_MAX);
        if (buf == NULL)
            errExit("malloc");

        s = pthread_setspecific(basename_key, buf);
        if (s != 0)
            errExitEN(s, "pthread_setspecific");
    }

    char *cur, *slash;
    slash = NULL;
    for (cur = path; *cur != '\0'; ++cur)
    {
        if (*cur == '/')
            slash = cur;
    }
    
    if (slash)
    {
        slash++;
        size_t len = cur-slash;
        len = (PATH_MAX-1 > len) ? len : PATH_MAX-1;
        strncpy(buf, slash, len);
        buf[len] = '\0';
    }
    else
    {
        size_t len = cur - path;
        len = (PATH_MAX-1 > len) ? len : PATH_MAX-1;
        strncpy(buf, path, len);
        buf[len] = '\0';
    }

    return buf;
}

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg)
{
    int s;

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    printf("%s\n", (char *)arg);
    printf("%s\n", dirname((char *)arg));
    printf("%s\n", basename((char *)arg));
    
    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int s;

    s = pthread_create(&t1, NULL, thread_func, "/home/cyyzero/workspece/std");
    if (s != 0)
    {
        errExitEN(s, "pthread_create");
    }

    s = pthread_create(&t2, NULL, thread_func, "/fuckasd/adfas/asdfa/asf/adf");
    if (s != 0)
    {
        errExitEN(s, "pthread_create");
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}