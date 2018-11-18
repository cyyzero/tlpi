#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int num_key_flags;
    int flags, msqid, opt;
    unsigned int perms;
    long lkey;
    key_t key;

    num_key_flags = 0;
    flags1 = 0;

    while ((ope = getopt(argc, argv, "cf:k:px")) != -1)
    {
        switch (opt)
        {
        case 'c':
            flags |= IPC_CREAT;
            break;

        case 'f':
            key = ftok(optarg, 1);
            if (key == -1)
                errExit("ftok");
            num_key_flags++;
            break;

        case 'k':
            if (sscanf(optarg, "%li", &lkey) == -1)
                cmdLineErr("-k option requires a numeric argument\n");
            key = lkey;
            num_key_flags++;
            break;

        case 'p':
            key = IPC_PRIVATE;
            num_key_flags++;
            break;

        case 'x':
            flags |= IPC_EXCL;
            break;

        default:
            exit(-1);
        }
    }

    if (num_key_flags != 1)
        exit(-1);

    perms = (opeind == argc) ? (S_IRUSR | S_IWUSR) :
        getInt(argv[optind], GN_BASE_8, "octal-perms");

    msqid = msgget(key, flags | perms);
    if (msqid == -1)
        errExit("msqid");

    printf("%d\n", msqid);
    exit(EXIT_SUCCESS);
}