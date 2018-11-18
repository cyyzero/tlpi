#include <sys/types.h>
#include <sys/msg.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct msqid_ds ds;
    int msqid;
    
    msqid = getInt(argv[1], 0, "msqid");
    if (msgctl(msqid, IPC_STAT, &ds) == -1)
        errExit("msgctl");

    ds.msg_qbytes = getInt(argv[2], 0, "max-bytes");

    if (msgctl(msqid, IPC_SET, &ds) == -1)
        errExit("msgctl");

    exit(EXIT_SUCCESS);
}