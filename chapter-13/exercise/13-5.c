#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUFFER_SIZE 4096


void count_lines(const char *s, int off, int *offsets, int number_lines, int* current_index)
{
    char *ch;
    int i = 0;
    int pre_i = 0;
    for (; s[i] != '\0'; ++i)
    {
        if (s[i] == '\n')
        {
            offsets[(*current_index)%number_lines] = off + i;
            ++*current_index;
            //printf("%d :%d\n", current_index, off+i);
        }
    }
}

int main(int argc, char *argv[])
{
    int *offsets;
    int fd, current_index = 0, number_lines = 10;
    const char *file_name;
    char buffer[BUFFER_SIZE];
    ssize_t number_bytes;

    if (argc != 2 && argc != 3)
        usageErr("tail [number] file\n");
    if (argc == 3)
    {
        number_lines = atoi(argv[1]);
        file_name = argv[2];
    }
    else
    {
        file_name = argv[1];
    }

    offsets = malloc(sizeof(int) * number_lines);
    if (offsets == NULL)
        errExit("malloc");
    memset(offsets, 0, sizeof(int) * number_lines);

    fd = open(file_name, O_RDONLY);
    if (fd == -1)
        errExit("open");


    int cnt = 0;
    while ((number_bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        //printf("cnt :%d\n", cnt);
        //puts(buffer);
        count_lines(buffer, cnt * BUFFER_SIZE, offsets, number_lines, &current_index);
        ++cnt;
    }
    // for(int i = 0; i < number_lines; ++i)
    //     printf("%d ", offsets[i]);
    // printf("curr : %d\n", current_index);
    // printf("\n");
    if (lseek(fd, (current_index >= number_lines) ? (offsets[(current_index+number_lines) % number_lines] + 1) : current_index, SEEK_SET) == -1)
        errExit("lseek");
    while ((number_bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
        write(STDOUT_FILENO, buffer, number_bytes);

    free(offsets);

    exit(EXIT_SUCCESS);
}