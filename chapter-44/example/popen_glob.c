#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include "print_wait_status.h"
#include "tlpi_hdr.h"

#define POPEN_FMT "/bin/ls -d %s 2> /dev/null"
#define PAT_SIZE 50
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PAT_SIZE)

int main(int argc, char *argv[])
{
    char pat[PAT_SIZE];
    char popen_cmd[PCMD_BUF_SIZE];
    FILE *fp;
    bool bad_pattern = false;
    int len, status, file_cnt, j;
    char pathname[PATH_MAX];

    for (;;)
    {
        printf("pattern: ");
        fflush(stdout);
        if (fgets(pat, PAT_SIZE, stdin) == NULL)
            break;
        len = strlen(pat);
        if (len <= 1)
            continue;

        if (pat[len - 1] == '\n')
            pat[len - 1] = '\0';

        for (j = 0, bad_pattern = false; j < len && !bad_pattern; ++j)
        {
            if (!isalnum((unsigned char)pat[j]) && strchr("_*?[^-].", pat[j]) == NULL)
                bad_pattern = true;
        }

        if (bad_pattern)
        {
            printf("Bad pattern character: %c\n", pat[j-1]);
            continue;
        }

        snprintf(popen_cmd, PCMD_BUF_SIZE, POPEN_FMT, pat);
        popen_cmd[PCMD_BUF_SIZE-1] = '\0';
        fp = popen(popen_cmd, "r");
        if (fp == NULL)
        {
            printf("popen() failed\n");
            continue;
        }

        file_cnt = 0;
        while (fgets(pathname, PATH_MAX, fp) != NULL)
        {
            printf("%s", pathname);
            ++file_cnt;
        }

        status = pclose(fp);
        printf("    %d matching file%s\n", file_cnt, file_cnt != 1 ? "s" : "");
        printf("    pclose() status == %#x\n", (unsigned)status);
        if (status != -1)
            print_wait_status("\t", status);
    }

    exit(EXIT_SUCCESS);
}