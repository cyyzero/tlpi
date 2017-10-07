/*
* 练习 4-2
* 简单实现cp，命令格式  cp [src-file] [dest-file]
* cp - copy files and directories
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif 

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fputs("argements error\n", stderr);
		exit(EXIT_FAILURE);	
	}
	int fdRead, fdWrite;
	ssize_t numRead;
	if ((fdRead = open(argv[1], O_RDONLY)) == -1)
	{
		fprintf(stderr, "Open \"%s\" error.\n", argv[1]);		
		exit(EXIT_FAILURE);
	}

	if ((fdWrite = open(argv[2], O_WRONLY | O_CREAT)) == -1)
	{
		fprintf(stderr, "Open \"%s\" error.\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	char buf[BUF_SIZE];
	while ((numRead = read(fdRead, buf, BUF_SIZE)) > 0)
	{
		write(fdWrite, buf, numRead);
	}

	if (close(fdRead) == -1)
	{
		fprintf(stderr, "Close file \"%s\" error.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (close(fdWrite) == -1)
	{
		fprintf(stderr, "Close file \"%s\" error.\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}