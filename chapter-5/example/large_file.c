/***************************************************************************
编译程序时候定义_LARGEFILE64_SOURCE功能测试宏，以使用过度型的LFS API。
该API所属函数具有处理64位文件大小和文件偏移量的能力。通常在32位版本尾部缀以64以示区别。
不过这是过渡型API，不推荐使用
推荐使用#define _FILE_OFF_SETBITS 64，就无需显式使用64位函数版本 
***************************************************************************/
#define _LARGEFILE64_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char **argv)
{
	int fd;
	off64_t off;
	if (argc != 3 || strcmp(argv[1]. "--help") == 0)
	{
		usageErr("%s pathname offset\n", argv[0]);
	}

	fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	(fd == -1)
		errExit("open64");

	off = atoll(argv[2]);
	if (lseek64(fd, off, SEEK_SET) == -1)
		errExit("lseek64");
	
	if (write(fd, "test", 4) == -1)
		errExit("write");
	exit(EXIT_SUCCESS);
}