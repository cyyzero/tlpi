/**********************************************************************
 * 用read() write()函数来模拟readv()和writev().
 * myreadv的实现是是循环iovcnt次，每次调用read。在语义上不同于单次的readv调用，
 * 因为发起多次write无法保证原子性。而且多次write()开销比一次writev()大。
 * mywritev的实现是利用malloc函数申请一块内存作为buffer，然后将iov中的数据都copy
 * 到buffer，最后从调用write。虽然从语义上等同于单词writev调用，但需要在用户控件内
 * 分配缓冲区，复制数据，很不方便（效率也低）。
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct iovec {
	void *iov_base;      /* Starting address */
	size_t iov_len;      /* Number of bytes to transfer */
};

ssize_t myreadv(int fd, const struct iovec* iov, int iovcnt);
ssize_t mywritev(int fd, const struct iovec* iov, int iovcnt);
int main(int argc, char **argv)
{
	int fd = open("t.txt", O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
	if (fd == -1)
	{
		exit(EXIT_FAILURE);
	}
	struct iovec text[2];
	char lorem[] = "lorem\n";
	text[0].iov_base = lorem;
	text[0].iov_len = 6;
	char ipsum[] = "IPSUM\n";
	text[1].iov_base = ipsum;
	text[1].iov_len = 6;
	ssize_t bytesWritten = mywritev(fd, text, 2);
	if (bytesWritten == -1)
		exit(EXIT_FAILURE);
	
	int fd1 = open("h.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	ssize_t bytesRead = myreadv(fd1, text, 2);
	if (bytesRead == -1)
		exit(EXIT_FAILURE);
	
	bytesWritten = mywritev(1, text, 2);
	if (bytesWritten == -1)
		exit(EXIT_FAILURE);
	return 0;
}

ssize_t myreadv(int fd, const struct iovec* iov, int iovcnt)
{
	ssize_t tot;
	for (int i = 0; i < iovcnt; ++i)
	{
		ssize_t size = read(fd, iov[i].iov_base, iov[i].iov_len);
		if (size == -1)
			exit(EXIT_FAILURE);
		if (size == 0)
			break;
		tot += size;
	}
	return tot;
}

ssize_t mywritev(int fd, const struct iovec* iov, int iovcnt)
{
	size_t size = 0;
	for (int i = 0; i < iovcnt; ++i)
	{
		size += iov[i].iov_len;
	}
	char *buf = (char *)malloc(size);
	
	ssize_t index = 0;
	for (int i = 0; i < iovcnt; ++i)
	{
		memcpy(&buf[index], iov[i].iov_base, iov[i].iov_len);
		index += iov[i].iov_len;
	}

	ssize_t bytesWritten = write(fd, buf, index);
	free(buf);
	return bytesWritten;
}