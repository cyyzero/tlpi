#include <sys/time.h>

unsigned int alarm(unsigned int seconds)
{
    struct itimerval new_value;
    struct itimerval old_value;

    new_value.it_value.tv_sec = seconds;
    new_value.it_value.tv_usec = 0;
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_sec = 0;
    setitimer(ITIMER_REAL, &new_value, &old_value);

    return old_value.it_value.tv_sec;
}