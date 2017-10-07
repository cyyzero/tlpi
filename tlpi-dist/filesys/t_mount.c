/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 14-1 */

/* t_mount.c

   Demonstrate the use of mount(2) to create a mount point.

   Usage: as described in usageError()

   Examples:
        t_mount -t ext3 /dev/sda12 /testfs

        t_mount -t ext2 -f r -o nogrpid /dev/sda9 /mydir ext2
                (The "r" for flags specifies that the file system
                is to be mounted read-only.)

   This program is Linux-specific.
*/
#include <sys/mount.h>
#include "tlpi_hdr.h"

#ifndef MS_DIRSYNC      /* May not be defined in older glibc headers */
#define MS_DIRSYNC 128
#endif

#ifndef MS_BIND         /* May not be defined in older glibc headers */
#define MS_BIND 4096
#endif

#ifndef MS_MOVE         /* May not be defined in older glibc headers */
#define MS_MOVE 8192
#endif

#ifndef MS_REC          /* May not be defined in older glibc headers */
#define MS_REC 16384
#endif

#ifndef MS_UNBINDABLE   /* May not be defined in older glibc headers */
#define MS_UNBINDABLE (1<<17) /* change to unbindable */
#endif

#ifndef MS_PRIVATE      /* May not be defined in older glibc headers */
#define MS_PRIVATE (1<<18) /* change to private */
#endif

#ifndef MS_SLAVE        /* May not be defined in older glibc headers */
#define MS_SLAVE (1<<19) /* change to slave */
#endif

#ifndef MS_SHARED       /* May not be defined in older glibc headers */
#define MS_SHARED (1<<20) /* change to shared */
#endif

#ifndef MS_LAZYTIME
#define MS_LAZYTIME     (1<<25)
#endif

static void
usageError(const char *progName, const char *msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s", msg);

    fprintf(stderr, "Usage: %s [options] source target\n\n", progName);
    fprintf(stderr, "Available options:\n");
#define fpe(str) fprintf(stderr, "    " str)    /* Shorter! */
    fpe("-t fstype        [e.g., 'ext2' or 'reiserfs']\n");
    fpe("-o data          [file system-dependent options,\n");
    fpe("                 e.g., 'bsdgroups' for ext2]\n");
    fpe("-f mountflags    can include any of:\n");
#define fpe2(str) fprintf(stderr, "            " str)
    fpe2("b - MS_BIND         create a bind mount\n");
    fpe2("d - MS_DIRSYNC      synchronous directory updates\n");
    fpe2("l - MS_MANDLOCK     permit mandatory locking\n");
    fpe2("L - MS_LAZYATIME    lazy atime updates\n");
    fpe2("m - MS_MOVE         atomically move subtree\n");
    fpe2("A - MS_NOATIME      don't update atime (last access time)\n");
    fpe2("V - MS_NODEV        don't permit device access\n");
    fpe2("D - MS_NODIRATIME   don't update atime on directories\n");
    fpe2("E - MS_NOEXEC       don't allow executables\n");
    fpe2("S - MS_NOSUID       disable set-user/group-ID programs\n");
    fpe2("p - MS_PRIVATE      mark as private\n");
    fpe2("r - MS_RDONLY       read-only mount\n");
    fpe2("c - MS_REC          recursive mount\n");
    fpe2("T - MS_RELATIME     relative atime updates\n");
    fpe2("R - MS_REMOUNT      remount\n");
    fpe2("h - MS_SHARED       mark as shared\n");
    fpe2("v - MS_SLAVE        mark as slave\n");
    fpe2("s - MS_SYNCHRONOUS  make writes synchronous\n");
    fpe2("u - MS_UNBINDABLE   mark as unbindable\n");
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    unsigned long flags;
    char *data, *fstype;
    int j, opt;

    flags = 0;
    data = NULL;
    fstype = NULL;

    while ((opt = getopt(argc, argv, "o:t:f:")) != -1) {
        switch (opt) {
        case 'o':
            data = optarg;
            break;

        case 't':
            fstype = optarg;
            break;

        case 'f':
            for (j = 0; j < strlen(optarg); j++) {

                /* In this version of the program we support more flags than
                   in the version of the program shown in the book */

                switch (optarg[j]) {
                case 'b': flags |= MS_BIND;             break;
                case 'd': flags |= MS_DIRSYNC;          break;
                case 'l': flags |= MS_MANDLOCK;         break;
                case 'm': flags |= MS_MOVE;             break;
                case 'A': flags |= MS_NOATIME;          break;
                case 'V': flags |= MS_NODEV;            break;
                case 'D': flags |= MS_NODIRATIME;       break;
                case 'E': flags |= MS_NOEXEC;           break;
                case 'S': flags |= MS_NOSUID;           break;
                case 'p': flags |= MS_PRIVATE;          break;
                case 'r': flags |= MS_RDONLY;           break;
                case 'c': flags |= MS_REC;              break;
                case 'T': flags |= MS_RELATIME;         break;
                case 'R': flags |= MS_REMOUNT;          break;
                case 'h': flags |= MS_SHARED;           break;
                case 'v': flags |= MS_SLAVE;            break;
                case 's': flags |= MS_SYNCHRONOUS;      break;
                case 'u': flags |= MS_UNBINDABLE;       break;
                default:  usageError(argv[0], NULL);
                }
            }
            break;

        default:
            usageError(argv[0], NULL);
        }
    }

    if (argc != optind + 2)
        usageError(argv[0], "Wrong number of arguments\n");

    if (mount(argv[optind], argv[optind + 1], fstype, flags, data) == -1)
        errExit("mount");

    exit(EXIT_SUCCESS);
}
