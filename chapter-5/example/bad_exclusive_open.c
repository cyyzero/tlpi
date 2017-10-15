/******************************************************************
$ ./bad_exclusive_open tfile sleep &
$ ./bad_exclusive_open tfile
会发现两个进程都认为是自己打开了tfile文件
（这里是使用了sleep，实际进程调度时间很短，不过仍有可能发生竞争状态）
*******************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "tlpi_hdr.h"
#include "error_functions.h"
int main(int argc, char *argv[])
{
	pid_t fd = open(argv[1], O_WRONLY);
	if( fd != -1) {
		printf("[PID %ld] File \"%s\" already exists\n",
			(long)getpid(), argv[1]);
		close(fd);
	} else {
		if (errno != ENOENT) {
			errExit("open");
		} else {
			printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long)getpid(), argv[1]);
			if (argc > 2) {
				sleep(5);
				printf("[PID %ld] Done sleeping\n", (long)getpid());
			}
			fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd == -1)
				errExit("open");
			printf("[PID %ld] Creat file \"%s\" exclusively\n",
				(long)getpid(), argv[1]);
			}
	}
	return 0;
}