#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include "tlpi_hdr.h"

key_t my_ftok(const char *pathname, int proj_id)
{
    key_t key;
    struct stat st;

    if (stat(pathname, &st) == -1)
    {
        key = -1;
    }
    else
    {
        key = 0;
        // printf("%x\n", proj_id&0xff);
        key |= (proj_id & 0xff);
        // printf("%x\n", key);
        key <<= 8;
        // printf("%x\n", st.st_dev&0xff);
        key |= (st.st_dev & 0xff);
        // printf("%x\n", key);
        key <<= 16;
        // printf("%x\n", st.st_ino&0xffff);
        key |= (st.st_ino & 0xffff);
        // printf("%x\n", key);
    }
    return key;
}

int main(void)
{
    // printf("%x\n%x\n", my_ftok("./45-2.c", 666), ftok("./45-2.c", 666));
    assert(my_ftok("./45-2.c", 666) == ftok("./45-2.c", 666));
}