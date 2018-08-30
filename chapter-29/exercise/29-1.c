/*
* 线程 join 自身会导致运行时错误 ERROR [EDEADLK/EDEADLOCK Resource deadlock avoided] pthread_join
* 在实际编程中应该避免这种情况。比如说在发起 join 操作的时候应该保证第一个参数与 pthread_self() 返回值不同
*/

#include <pthread.h>
#include "tlpi_hdr.h"

void *pthread_func(void *arg)
{
    printf("Pthread t1 is running.\n");
}

int main(int argc, char *argv[])
{
    pthread_t t1;
    int s;

    s = pthread_create(&t1, NULL, pthread_func, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    // It occurs a runtime error: ERROR [EDEADLK/EDEADLOCK Resource deadlock avoided] pthread_join
    // s = pthread_join(pthread_self(), NULL);
    // if (s != 0)
    //     errExitEN(s, "pthread_join");

    printf("Pthread t1 is over.\n");

    return 0;

}