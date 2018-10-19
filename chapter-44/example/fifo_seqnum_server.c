#include <signal.h>
#include "fifo_seqnum.h"

int main(int argc, char *argv[])
{
    int server_fd, dummy_fd, client_fd;
    char client_fifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int seq_num = 0;

    umask(0);

    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
        errExit("mkfifo %s", SERVER_FIFO);

    server_fd = open(SERVER_FIFO, O_RDONLY);
    if (server_fd == -1)
        errExit("open %s", SERVER_FIFO);

    dummy_fd = open(SERVER_FIFO, O_WRONLY);
    if (dummy_fd == -1)
        errExit("open %s", SERVER_FIFO);

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        errExit("signal");

    for (;;)
    {
        if (read(server_fd, &req, sizeof(struct request)) != sizeof(struct request))
        {
            fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }

        snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)req.pid);
        client_fd = open(client_fifo, O_WRONLY);
        if (client_fd == -1)
        {
            errMsg("open %s", client_fifo);
            continue;
        }

        resp.seq_num = seq_num;
        if (write(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
            fprintf(stderr, "Error writing to FIFO %s\n", client_fifo);
        if (close(client_fd) == -1)
            errMsg("close");

        seq_num += req.seq_len;
    }
}