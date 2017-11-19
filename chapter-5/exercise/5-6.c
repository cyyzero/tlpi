/*******************************************************************************
 * 内核维护了三个数据结构：
 * 	1.进程级的文件描述符表
 * 	2.系统级的打开文件表
 * 	3.文件系统的i-node表
 * 成功调用dup的话（man上的资料）：They （指的是newfd和oldfd） refer  to  the same open file description and 
 *      thus share file offset and file status flags.
 * 	也就是说公用打开文件表中的一项，从而偏移量相同。
 * 而对同一个文件调用两次open的话，打开文件表中会出现两项
 * 从而最后结果是写入了 Giddayworld
********************************************************************************/
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int fd1, fd2, fd3;
	const char *file = "test.txt";
	fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	fd2 = dup(fd1);
	fd3 = open(file, O_RDWR);
	write(fd1, "Hello,", 6);
	write(fd2, "world", 6);
	lseek(fd2, 0, SEEK_SET);
	write(fd1, "HELLO,", 6);
	write(fd3, "Gidday", 6);
	return 0;
}