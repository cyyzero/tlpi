#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    struct stat sb;

    for (int i = 1; i < argc; ++i)
    {
        if (stat(argv[i], &sb) == -1)
        {
            perror("Fuck U");
            continue;
        }

        sb.st_mode |= (S_IRUSR | S_IRGRP | S_IROTH);

        if (S_ISDIR(sb.st_mode) || 
            (sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
        {
            sb.st_mode |= (S_IXUSR | S_IXGRP | S_IXOTH);
        }
        chmod(argv[i], sb.st_mode);
    }
    return 0;
}