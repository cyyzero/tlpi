/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 57-8 */

/* us_abstract_bind.c

   Demonstrate how to bind a UNIX domain socket to a name in the
   Linux-specific abstract namespace.

   This program is Linux-specific.

   The first printing of the book used slightly different code. The code was
   correct, but could have been better (to understand why, see the errata
   for page 1176 of the book).  The old code is shown in comments below.
*/
#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_un addr;
    char *str;

    memset(&addr, 0, sizeof(struct sockaddr_un));  /* Clear address structure */
    addr.sun_family = AF_UNIX;                     /* UNIX domain address */

    /* addr.sun_path[0] has already been set to 0 by memset() */

    str = "xyz";        /* Abstract name is "\0abc" */
    strncpy(&addr.sun_path[1], str, strlen(str));

    // In early printings of the book, the above two lines were instead:
    //
    // strncpy(&addr.sun_path[1], "xyz", sizeof(addr.sun_path) - 2);
    //            /* Abstract name is "xyz" followed by null bytes */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
        errExit("socket");

    if (bind(sockfd, (struct sockaddr *) &addr,
            sizeof(sa_family_t) + strlen(str) + 1) == -1)
        errExit("bind");

    // In early printings of the book, the final part of the bind() call
    // above was instead:
    //        sizeof(struct sockaddr_un)) == -1)

    sleep(60);

    exit(EXIT_SUCCESS);
}
