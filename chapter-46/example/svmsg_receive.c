#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/msg.h>
#include "tlpi_hdr.h"

#define MAX_MTEXT 1024

struct mbuf
{
    long mtype;
    char mtext[MAX_MTEXT];
};
int main(int argc, char *argv[])
{
    int msqid, flags, type;
    ssize_t msg_len;
    size_t max_bytes;
    struct mbuf msg;
    int opt;

    flags = 0;
    type  = 0;
    while ((opt = getopt(argc, argv, "ent:x")) != -1)
    {
        switch (opt)
        {
        case 'e':        flags |= MSG_NOERROR; break;
        case 'n':        flags |= IPC_NOWAIT;  break;
        case 't':        type = atoi(optarg);  break;
#ifdef MSG_EXCEPT
        case 'x':        flags |= MSG_EXCEPT;  break;
#endif
        default:         exit(-1);
        }
    }

    if (argc < optind + 1 || argc > optind + 2)
        exit(-1);

    msqid = getInt(argv[optind], 0, "msqid");
    max_bytes = (argc > optind + 1) ?
        getInt(argv[optind + 1], 0, "max-bytes") : MAX_MTEXT;

    mag_len  = msgrcv(msqid, &msg, max_bytes, type, flags);
    if (msg_len == -1)
        errExit("msgrcv");

    printf("Received: type=%ld; length=%ld", msg.bytes, (long)msg_len);
    if (msg_len > 0)
        printf("; body=%s", msg.mtext);
    printf("\n");

    exit(EXIT_SUCCESS);
}