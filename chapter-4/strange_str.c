#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_READ 20

int main(int argc, char* argv[])
{
	char buffer[MAX_READ];
/*	if(read(STDIN_FILENO, buffer, MAX_READ) == -1)
	{
		fputs("read", stderr);
		_exit(-1);
	}
*/	
//输出的结果可能会有点带有其他字符，因为read函数不会给buffer末尾添加/0
//因为输入信息可能是其他二进制形式的数据结构，read()无法区分这些数据
//所以无法遵从C语言对字符串处理的约定,正确方法如下

	ssize_t numRead;
	numRead = read(STDIN_FILENO, buffer, MAX_READ);
	if (numRead == -1) {
		fputs("read", stderr);
		_exit(EXIT_FAILURE);
	}
	buffer[numRead] = '\0';
	printf("The input data was: %s\n", buffer);
	return 0;
}

