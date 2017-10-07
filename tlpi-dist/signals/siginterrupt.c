/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Solution for Exercise 20-4 */

/* siginterrupt.c

   An implementation of the siginterrupt(3) library function.
*/
#include <stdio.h>
#include <signal.h>

int
siginterrupt(int sig, int flag)
{
    int status;
    struct sigaction act;

    status = sigaction(sig, NULL, &act);
    if (status == -1)
        return -1;

    if (flag)
        act.sa_flags &= ~SA_RESTART;
    else
        act.sa_flags |= SA_RESTART;

    return sigaction(sig, &act, NULL);
}
