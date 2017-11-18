/*********************************
使用Ｏ_APPEND标志打开一个已存在的文件，
且将稳健偏移量置于文件起始处。再写入数据，
数据会接在原文件后。这是因为 对于lseek来
说，它直接修改文件描述符表项中的当前文件偏
移量，并返回当前的文件偏移量，而对于O_APPEND
标志，则只是将其设置到了文件表项的文件状态
标志中，此时当前文件偏移量并未修改，仍然为
0，只有当用户企图写入文件时，才将文件当前
偏移量置为文件长度。
*********************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd;
	off_t off;
	ssize_t numWritten;

	if (argc  != 2 || strcmp(argv[1], "--help") == 0)
	{
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		exit(EXIT_FAILURE);
	}

	off = lseek(fd, 0, SEEK_SET);
	if (off == -1)
		exit(EXIT_FAILURE);
	
	numWritten = write(fd, "f**k\n", 5);
	if (numWritten == -1)
		exit(EXIT_FAILURE);
	close(fd);
	return 0;
}
