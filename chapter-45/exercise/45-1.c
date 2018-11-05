#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tlpi_hdr.h"

const char* FILE_PATH = "45-1.c";

int main(void)
{
    struct stat st;
    key_t key;
    int proj_id;

    if (stat(FILE_PATH, &st) == -1)
        errExit("stat");

    proj_id = 0x1234;
    key = ftok(FILE_PATH, proj_id);

    printf("key:    %x\n",  key);
    printf("inode:  %lx\n", st.st_ino);
    printf("device: %lx\n", st.st_dev);
    printf("proj_id:%x\n",  proj_id);

    /*
    key 比特示意  |31|30|...|1|0|
    key 31-24位是prod_id的低8位，23-16位是次设备号，即st_dev的低8位，15-0是i节点的低16位。 
    key |= (proj_id & 0xff);
    key <<= 8;
    key |= (st.st_dev & 0xff);
    key <<= 16;
    key |= (st.st_ino & 0xffff);
    */
}