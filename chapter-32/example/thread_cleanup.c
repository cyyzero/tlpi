#include <pthread.h>
#include "tlpi_hdr.h"

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int glob = 0;

static void cleanup_handler(void *arg)
{
    int s;

    printf("Cleanup: freeing block at %p\n", arg);
    free(arg);

    printf("cleanup: unlocking mutex\n");

    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");
}

static void *thread_func(void *arg)
{
    int s;

    void *buf = NULL;
    buf = malloc(0x10000);
    printf("thread: allocated memory at %p\n", buf);

    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    pthread_cleanup_push(cleanup_handler, buf);

    while (glob == 0)
    {
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0)
            errExitEN(s, "pthread_cond_wait");
    }

    printf("thread: condition wait loop completed\n");

    pthread_cleanup_pop(1);
    
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    void *res;
    int s;
    s = pthread_create(&thr, NULL, thread_func, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    sleep(2);

    if (argc == 1)
    {
        printf("main:    about to cancel thread\n");
        s = pthread_cancel(thr);
        if (s != 0)
            errExitEN(s, "pthread");
    }
    else
    {
        printf("main:    about to signal conditation variable\n");
        glob = 1;
        s = pthread_cond_signal(&cond);
        if (s != 0)
            errExitEN(s, "pthread_cond_signal");
    }

    s = pthread_join(thr, &res);
    if (s != 0)
        errExitEN(s, "pthread_join");

    if (res == PTHREAD_CANCELED)
        printf("main:    thread was canceled\n");
    else
        printf("main:    thread terminated normally\n");

    exit(EXIT_SUCCESS);
}