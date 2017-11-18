#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[]){
    int i, fd, flags, numBytes;
    off_t off;
    ssize_t numWritten;


    flags = O_RDWR | O_CREAT;
    if(argc < 3 || strcmp(argv[1], "--help") == 0 )
        usageErr("%s filename num-bytes [x]");

    if(argc != 4)
        flags = flags | O_APPEND;

    numBytes = getInt(argv[2], 0, "num-bytes");

    fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if(fd == -1)
        errExit("open");

    /*if(argc == 4)
        if(lseek(fd, 0, SEEK_END) == -1)
            errExit("lseek");

    */

    for(i = 0; i < numBytes; ++i){
        if(argc > 3 && strcmp(argv[3],"x") == 0)
            if(lseek(fd, 0, SEEK_END) == -1)
                errExit("lseek");

        if(write(fd, "A", 1) != 1)
            fatal("write() failed");
    }
        
    exit(EXIT_SUCCESS);
}