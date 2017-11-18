/******************************************
 * 用fcntl实现dup和dup2
 * 用fcntl(oldfd, F_GETFL) 可以检查oldfd是否打开，
 * 未打开返回-1
*******************************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int mydup(int oldfd);
int mydup2(int oldfd, int newfd);
int main(int argc, char **argv)
{
	int fd, newfd;

	newfd = mydup(0);

	printf("old fd is 0, new fd is %d\n", newfd);

	newfd = mydup2(0, 4);

	printf("old fd is 0, new fd is %d\n", newfd);

	return 0;
}

int mydup(int oldfd)
{
	return fcntl(oldfd, F_DUPFD, 0);
}

int mydup2(int oldfd, int newfd)
{
	if (newfd < 0)
	{
		errno = EBADF;
		return -1;
	}

	if (fcntl(oldfd, F_GETFL) == -1)
		return -1;

	if (oldfd == newfd)
		return old;
	
	close(newfd);

	return fcntl(oldfd, F_DUPFD, newfd);
}