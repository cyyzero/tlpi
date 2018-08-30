#define _BSD_SOURCE
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void run(pid_t (*fun)(void), size_t cnt)
{
    int status;
    clock_t begin, end;

    begin = clock();
    for (int i = 0; i < cnt; ++i)
    {

        switch (fun())
        {
        case -1:
            exit(EXIT_FAILURE);
        case 0:
            _exit(EXIT_SUCCESS);
        default:
            wait(&status);
            break;
        }
    }
    end = clock();
    printf("%lf\n", (double)(end-begin)/CLOCKS_PER_SEC);
}

int main(void)
{
    const size_t times = 10000;
    printf("fork()  %llu :", (unsigned long long)times);
    run(fork, times);

    printf("vfork() %llu :", (unsigned long long)times);
    run(vfork, times);
    return 0;
}