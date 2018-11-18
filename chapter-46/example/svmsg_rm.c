#include <sys/types.h>
#include <sys/msg.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j;
    
    for (j = 1; j < argc; ++j)
        if (msgctl(getInt(argv[j], 0, "msqid"), IPC_RMID, NULL) == -1)
        {
            errExit("msgctl %s", argv[j]);
        }

    exit(EXIT_SUCCESS);
}