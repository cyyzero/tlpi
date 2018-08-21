#define _XOPEN_SOURCE
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int sig, sighandler_t handler)
{
    struct sigaction new_disp, prev_disp;

    new_disp.sa_handler = handler;
    sigemptyset(&new_disp.sa_mask);
#ifdef OLD_SIGNAL
    new_disp.sa_flags = SA_RESETHAND | SA_NODEFER;
#else
    new_disp.sa_flags = SA_RESTART;
#endif

    if (sigaction(sig, &new_disp, &prev_disp) == -1)
        return SIG_ERR;
    else
        return prev_disp.sa_handler;
}