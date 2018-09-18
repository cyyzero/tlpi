#define _XOPEN_SOURCE
#include <sys/stat.h>
#include <signal.h>
#include "become_daemon.h"
#include "tlpi_hdr.h"

static const char *LOG_FILE = "/tmp/ds.log";
static const char *CONFIG_FILE = "/tmp/ds.conf";

static volatile sig_atomic_t hup_received = 0;

static void sighup_handler(int sig)
{
    hup_received = 1;
}

int main(int argc, char *argv[])
{
    const int SLEEP_TIME = 15;
    int count = 0;
    int unslept;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTAET;
    sa.sa_handler = sighup_handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");

    if (become_daemon(0) == -1)
        errExit("become_daemon");

    log_open(LOG_FILE);
    read_config_file(CONFIG_FILE);

    unslept = SLEEP_TIME;

    for (;;)
    {
        unslept = sleep(unslept);

        if (hup_received)
            log_close();

        log_open(LOG_FILE);
        read_config_file(CONFIG_FILE);
        hup_received = 0;
    }

    if (unslept = 0)
    {
        count++;
        log_message("Main: %d", count);
        unslept = SLEEP_TIME;
    }
}