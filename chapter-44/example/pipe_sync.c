#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int pfd[2];
    int j, dummy;

    setbuf(stdout, NULL);

    printf("Parent started\n");

    if (pipe(pfd) == -1)
        errExit("pipe");

    for (j = 1; j < argc; ++j)
    {
        switch (fork())
        {
        case -1:
            errExit("fork");

        case 0:
            if (close(pfd[0]) == -1)
                errExit("close");

            sleep(getInt(argv[j], GN_NONNEG, "sleep-time"));

            printf("Chid %d (PID=%ld) closing pipe\n", j, (long)getpid());
            if (close(pfd[1]) == -1)
                errExit("close");
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    if (close(pfd[1]) == -1)
        errExit("close");

    if (read(pfd[0], &dummy, 1) != 0)
        fatal("parent didn't get EOF");

    printf("Parent ready to go\n");

    exit(EXIT_SUCCESS);
}