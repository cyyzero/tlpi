#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include "tlpi_hdr.h"
struct init_info
{
    bool has_called;
    pthread_mutex_t mtx;
};

static struct init_info control = { false, PTHREAD_MUTEX_INITIALIZER };

int one_time_init(struct init_info *control, void (*init)(void))
{
    int s;

    s = pthread_mutex_lock(&control->mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_lock");

    if (!control->has_called)
    {
        init();
        control->has_called = true;
    }

    s = pthread_mutex_unlock(&control->mtx);
    if (s != 0)
        errExitEN(s, "pthread_mutex_unlock");
}

void initialize(void)
{
    printf("Initialize() is being called.\n");
}

void *thread_func(void *arg)
{
    one_time_init(&control, initialize);
    printf("Thread %llu has been called.\n", (unsigned long long)pthread_self());
}

int main(void)
{
    const int THREAD_LEN = 10;
    int s;
    pthread_t t[THREAD_LEN];

    for (int i = 0; i < THREAD_LEN; ++i)
    {
        pthread_create(&t[i], NULL, thread_func, NULL);
        pthread_detach(t[i]);
    }

    exit(EXIT_SUCCESS);

}