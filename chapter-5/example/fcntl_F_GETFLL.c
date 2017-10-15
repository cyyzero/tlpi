/**************************
省略了前面的代码，已知一个变量fd，
保存了文件描述符，要判断访问模式
***************************/
	int flags, accessMode;
	flags = fcntl(fd, F_GETFL);
	if (flsgs == -1)
		exit(EXIT_FAILURE)；
	accessMode = flags & O_ACCMODE;
	if (accessMode == O_WRONLY || accessMode == O_RDWR)
		printf("file is writable\n");


