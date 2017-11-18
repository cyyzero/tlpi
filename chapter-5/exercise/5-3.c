/**********************************************************
 * 当文件以O_APPEND标志位打开时，能保证write操作的原子性。
 * 从语义上来说，和每次lseek到文件结尾，然后再write等价，但
 * 是这么做就不能保证write操作的原子性。
 * 比如说以下代码,编译后的可执行文件为 atomic_append
 * atomic_append filename num_bytes [x]
 * 该程序打开指定的文件（不存在就创建），然后每次调用write写入
 * 一个字节，共写入num_bytes个。缺省情况下，用O_APPEND打开，
 * 加参数x就每次sleek到文件末尾再写入。
 * $ atomic_append f1 1000000 & atomic_append f1 1000000
 * $ atomic_append f2 1000000 x & atomic_append f2 1000000 x
 * 最后测试结果第一次是完整写入2000000个字节。第二次却因为出现了覆盖(比
 * 如两个进程运行到sleek时另一方都还未write，即文件的pos相同，然后write
 * 就会覆盖)，写入了1000000+字节。
**********************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
int main(int argc, char ** argv)
{
    int fd;
    int size;
    int flag;
    bool append;
    ssize_t numWritten;
    off_t off;

    append = true;

    if (argc > 4)
    {
        fputs("Too many args\n", stderr);
        exit(EXIT_FAILURE);
        
    }
    if ((size = atoi(argv[2])) == 0)
    {
        fputs("The second arg is not a number or 0", stderr);
        exit(EXIT_FAILURE);
    }
    if (argc == 4)
    {
        if (strcmp("x", argv[3]) == 0)
            append = false;
        else
        {
            fputs("The third arg is not 'x'", stderr);
            exit(EXIT_FAILURE);
        }
    }

    flag = O_WRONLY|O_CREAT;
    if (append)
        flag |= O_APPEND;

    fd = open(argv[1], flag, S_IWUSR | S_IRUSR);

    if (append)
    {
        while (size--)
        {
            if (write(fd, "1", 1) == -1)
            {
                fputs("write error\n", stderr);
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        while (size--)
        {
            if (lseek(fd, 0, SEEK_END) == -1)
            {
                fputs("lseek error\n", stderr);
                exit(EXIT_FAILURE);
            }
            if (write(fd, "0", 1) == -1)
            {
                fputs("write error\n", stderr);
                exit(EXIT_FAILURE);
            }
        }
    }
    close(fd);
    return 0;
}
