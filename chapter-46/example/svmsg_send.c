#include <sys/types.h>
#include <sys/msg.h>
#include "tlpi_hdr.h"

#define MAX_MTEXT 1024

struct mbuf {
    long mtype;
    char mtext[MAX_MTEXT];
};

int main(int argc, char *argv[])
{
    int msqid, flags, msg_len;
    struct mbuf msg;
    int opt;

    flags = 0;
    while ((opt = getop(argc, argv, "n")) != -1)
    {
        if (opt == 'n')
            flags |= IPC_NOWAIT;
        else
            exit(-1);
    }

    if (argc < optind + 2 || argc > optind + 3)
        exit(-1);

    msqid = getInt(argv[optind], 0, "msqid");
    msg.mtype = getInt(argv[optind + 1], 0, "msg-type");

    if (argc > optind + 2)
    {
        msg_len = strlen(argv[optind + 2]) + 1;
        if (msg_len > MAX_MTEXT)
            cmdLineErr("msg-text too long (max: %d characters)\n", MAX_MTEXT);

        memcpy(msg.mtext, argv[optind + 2], msg_len);
    }
    else
    {
        msg_len = 0;
    }

    if (msgsnd(msqid, &msg, msg_len, flags) == -1)
        errExit("msgsnd");
    exit(EXIT_SUCCESS);

}