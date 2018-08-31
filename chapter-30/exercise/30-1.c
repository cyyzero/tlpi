#include <pthread.h>
#include "tlpi_hdr.h"

struct thread_info
{
    int thread_id;
    int loops;
};

static volatile int glob = 0;

static void *thread_func(void *arg)
{
    struct thread_info *p = (struct thread_info *)arg;
    int loc, j;

    for (j = 0; j < p->loops; ++j)
    {
        loc = glob;
        loc++;
        glob = loc;
        printf("Pthread id = %d; glob = %d\n", p->thread_id, glob);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    struct thread_info st1, st2;
    pthread_t t1, t2;
    int s;

    st1.thread_id = 1;
    st2.thread_id = 2;
    st1.loops = st2.loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    s = pthread_create(&t1, NULL, thread_func, &st1);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_create(&t2, NULL, thread_func, &st2);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("glob=%d\n", glob);
    exit(EXIT_SUCCESS);
}