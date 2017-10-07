/*
* 练习 4-1
* 简单实现tee命令,必须指明输入的文件，选项也只有-a
* tee - read from standard input and write to standard output and files
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

void print_usage(void);
int main(int argc, char  *argv[])
{
	int opt, append_flag = 0;
	if (argc != 2 && argc != 3)
	{
		fprintf(stderr, "%d arguments, error\n",argc);
		exit(EXIT_FAILURE);
	}
	while ( (opt = getopt(argc, argv, "a")) != -1 )
	{
		switch (opt)
		{
		case 'a':
			append_flag = 1;
			break;
		case 'h':
		case '?':
			print_usage();
			break;
		default:
			break;
		}
	}

	if (optind > argc)
		exit(EXIT_FAILURE);
	char buf[BUF_SIZE];
	ssize_t numRead;
	int flag = O_WRONLY | O_CREAT;
	if (append_flag)
		flag |= O_APPEND;
	else
		flag |= O_TRUNC;
	
	int fd = open(argv[optind], flag, 
		S_IRUSR | S_IWUSR | S_IRGRP |
		S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1)
	{
		fputs("open error", stderr);
		exit(EXIT_FAILURE);
	}
	while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
	{
		write(fd, buf, numRead);
		write(STDOUT_FILENO, buf, numRead);
	}

	if (numRead == -1)
	{
		fputs("read error", stderr);
		exit(EXIT_FAILURE);
	}

	if (close(fd) == -1)
	{
		fputs("close error", stderr);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

}

void print_usage(void)
{
	fputs("usage:\n-a append to the file\n-h or -? help\ndefalult:wirte a new file or overwrite the file\n", stderr);
	exit(EXIT_FAILURE);
}