/**************************************
 * 验证文件描述符及其副本共享了文件偏移量和打
 * 开文件的状态标志(file status flags)。
 * flags分为file status flags,file creation 
 * flags,file access flags。
***************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
void errExit(const char*);
int main(int argc, char **argv)
{

	int fd, newfd, flag, anotherflag;
	if (argc != 2)
	{
		errExit("Arguments error!");
	}
	flag = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(argv[1], flag, S_IRUSR | S_IWUSR);

	if (fd == -1)
	{
		errExit("Open error!");
	}
	newfd = dup(fd);
	if (newfd == -1)
		errExit("dup error!");

	anotherflag = fcntl(newfd, F_GETFL);
	if (O_WRONLY == (anotherflag & O_ACCMODE) && (anotherflag & O_APPEND))
	{
		printf("Flag is the same\n");
	}
	else 
		printf("Flag is not the same\n");

	if (write(fd, "Hello ", 6) == -1)
		errExit("Write error!");
	if (write(newfd, "world", 5) == -1)
		errExit("Write error");
	close(fd);
	close(newfd);
	return 0;
}
void errExit(const char* err)
{
	fputs(err, stderr);
	exit(EXIT_FAILURE);
}