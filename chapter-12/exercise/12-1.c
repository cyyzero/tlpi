#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <dirent.h>
#include "tlpi_hdr.h"

uid_t user_id_from_name(const char* name);
bool is_numbers(const char *name);
bool is_dirent(const char *name);
void read_status(const char *name, uid_t uid);

int main(int argc, char *argv[])
{
    char name[1024];
    DIR * proc;
    struct dirent *dirent;
    uid_t uid;

    if (argc != 2)
    {
        fprintf(stderr, "args error\n");
        exit(EXIT_FAILURE);
    }
    
    uid = user_id_from_name(argv[1]);
    if (uid == -1)
        errExit("user_id_from_name");
    printf("UID:%ld\n", (long)uid);

    proc = opendir("/proc");
    if (proc == NULL)
        errExit("opendir");
    
    while ((dirent = readdir(proc)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)
            continue;
        
        sprintf(name, "/proc/%s", dirent->d_name);
        if (is_dirent(name) && is_numbers(dirent->d_name))
        {
            strcat(name, "/status");
            read_status(name, uid);
        }
    }
    exit(EXIT_SUCCESS);
}

uid_t user_id_from_name(const char* name)
{
    struct passwd *pwd;

    if (name == NULL || *name == '\0')
        return -1;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

bool is_numbers(const char *name)
{
    while (*name != '\0')
    {
        if (!isdigit(*name))
            return false;
        ++name;
    }
    return true;
}

bool is_dirent(const char *name)
{
    struct stat s;
    if (stat(name, &s) == -1)
    {
        errExit("stat");
    }

    if ((s.st_mode & S_IFMT) == S_IFDIR)
        return true;
    else
        return false;
}

void read_status(const char *name, uid_t uid)
{
    FILE *file;
    char proc_name[30];
    char line[100];
    uid_t ruid, euid, suid, fsuid;

    file = fopen(name, "r");
    if (file == NULL)
        errExit("fopen");
    fscanf(file, "Name:\t%s", proc_name);
    for (int i = 0; i < 8; ++i) {
        //fgets()
        fgets(line, 100, file);
        //puts(line);
    }
    fscanf(file, "%s%d%d%d%d", line, &ruid, &euid, &suid, &fsuid);
    //printf("%d %d %d %d\n", ruid, euid, suid, fsuid);
    //puts(proc_name);
    if (ruid == uid || euid == uid || suid == uid || fsuid == uid)
        puts(proc_name);
}