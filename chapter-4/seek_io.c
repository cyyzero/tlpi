#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "tlpi_hdr.h"
int main(int argc, char *argv[])
{
	size_t len;
	off_t offset;
	int fd, ap, j;
	char *buf;
	ssize_t numRead, numWritten;

	if (argc < 3 || strcmp(argv[j], "--help") == 0)
		usageErr("%s file {r<length|R<length|w<string>|s<offset>}....n", argv[0]);
	fd = open(argv[1], O_RDWR | O_CREAT, 
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1)
		errExit("open");

	for (ap = 2; ap < argc; ++ap)
	{
		switch (argv[ap][0])
		{
			case 'r':    /*按文本处理*/
			case 'R':    /*按二进制处理*/
				len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
				buf = malloc(len);
				if (buf == NULL)
				{
					errExit("malloc");
				}

				numRead = read(fd, buf, len);
				if (numRead == -1)
				{
					errExit ("read");
				}

				if (numRead == 0)
					printf("%s: end-of-file\n", argv[1]);
				else {
					printf("%s: ", argv[ap]);
					for (j = 0; j < numRead; ++j)
					{
						if(argv[ap][0] == 'r')
							printf("%c", isprint(buf[j]) ? buf[j]: '?');
						else
							printf("%02x", buf[j]);
					}
					printf("\n");
				}
				free (buf);
				break;
			case 'w':
				numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
				if (numWritten == -1)
				{
					errExit("write");
				}
				printf("%s: worte %ld bytes\n", argv[ap], (long)numWritten);
				break;

			case 's':
				offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
				if (lseek(fd, offset, SEEK_SET) == -1)
					errExit("lseek");
				printf("%s: seek successed\n", argv[ap]);
				break;

			default:
				cmdLineErr("Argv must start with [rRws]: %s\n", argv[ap]);
		}
	}
	exit(EXIT_SUCCESS);
}