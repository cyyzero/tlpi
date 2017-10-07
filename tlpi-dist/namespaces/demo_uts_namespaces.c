/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter Z */

/* demo_uts_namespaces.c

   Demonstrate the operation of UTS namespaces.

   See https://lwn.net/Articles/531245/
*/
#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* A simple error-handling function: print an error message based
   on the value in 'errno' and terminate the calling process */

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

static int              /* Start function for cloned child */
childFunc(void *arg)
{
    struct utsname uts;

    /* Change hostname in UTS namespace of child */

    if (sethostname(arg, strlen(arg)) == -1)
        errExit("sethostname");

    /* Retrieve and display hostname */

    if (uname(&uts) == -1)
        errExit("uname");
    printf("uts.nodename in child:  %s\n", uts.nodename);

    /* Keep the namespace open for a while, by sleeping.
       This allows some experimentation--for example, another
       process might join the namespace. */

    sleep(1000);

    return 0;           /* Terminates child */
}

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

static char child_stack[STACK_SIZE];   /* Space for child's stack */

int
main(int argc, char *argv[])
{
    pid_t child_pid;
    struct utsname uts;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* Create a child that has its own UTS namespace;
       the child commences execution in childFunc() */

    child_pid = clone(childFunc,
                    child_stack + STACK_SIZE,   /* Points to start of
                                                   downwardly growing stack */
                    CLONE_NEWUTS | SIGCHLD, argv[1]);
    if (child_pid == -1)
        errExit("clone");
    printf("PID of child created by clone() is %ld\n", (long) child_pid);

    /* Parent falls through to here */

    sleep(1);           /* Give child time to change its hostname */

    /* Display the hostname in parent's UTS namespace. This will be
       different from the hostname in child's UTS namespace. */

    if (uname(&uts) == -1)
        errExit("uname");
    printf("uts.nodename in parent: %s\n", uts.nodename);

    if (waitpid(child_pid, NULL, 0) == -1)      /* Wait for child */
        errExit("waitpid");
    printf("child has terminated\n");

    exit(EXIT_SUCCESS);
}
