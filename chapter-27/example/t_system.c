#include <sys/wait.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define MAX_CMD_LEN 200

int main(int argc, char *argv[])
{
    char str[MAX_CMD_LEN];
    int status;

    for (;;)
    {
        printf("Command: ");
        fflush(stdout);
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
            break;
        
        status = system(str);
        printf("system() returned: status=%04x (%d,%d)\n",
               (unsigned int)status, status >> 8, status & 0xff);

        if (status== -1)
        {
            errExit("system");
        }
        else
        {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
            {
                printf("(Probably) could not ivoke shell\n");
            }
            else
            {
                print_wait_status(NULL, status);
            }
        }
    }
}