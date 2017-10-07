/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
\*************************************************************************/

/* Solution for Exercise 59-3:a */

/* unix_sockets.h

   Header file for unix_sockets.c.
*/
#ifndef UNIX_SOCKETS_H
#define UNIX_SOCKETS_H      /* Prevent accidental double inclusion */

#include <sys/socket.h>
#include <sys/un.h>

int unixBuildAddress(const char *path, struct sockaddr_un *addr);

int unixConnect(const char *path, int type);

int unixListen(const char *path, int backlog);

int unixBind(const char *path, int type);

#endif
