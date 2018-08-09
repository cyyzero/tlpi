#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include "tlpi_hdr.h"

#define SECONDS_IN_TROPIAL_YEAR (365.24219 * 24 * 60 * 60)

int main(int argc, char *argv[])
{
    time_t t;
    struct tm *gmp, *locp;
    struct tm gm, loc;
    struct timeval tv;

    t = time(NULL);
    printf("Seconds since the Epoch (1 Jan 1970): %ld", (long)t);
    printf(" (about %6.3f years)\n", t / SECONDS_IN_TROPIAL_YEAR);

    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");
    printf(" gettimeofday() returned %ld secs, %ld microsecs\n",
           (long)tv.tv_sec, (long)tv.tv_usec);
    
    gmp = gmtime(&t);
    if (gmp == NULL)
        errExit("gmtime");

    gm = *gmp;

    printf("Broken down by gmtime():\n");
    printf(" years=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year,
           gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n", gm.tm_mday, gm.tm_yday, gm.tm_isdst);

    locp = localtime(&t);
    if (locp == NULL)
        errExit("localtime");
    
    loc = *locp;
    printf("Broken down by localtime():\n");
    printf(" years=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc.tm_year,
           loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n", loc.tm_mday, loc.tm_yday, loc.tm_isdst);

    printf("asctime() formats the gmtime() value as: %s", asctime(&gm));
    printf("ctime() formats the time() value as:     %s", ctime(&t));

    printf("mktime() of gmtime() value:    %ld secs\n", (long)mktime(&gm));
    printf("mktime() of localtime() value: %ld secs\n", (long)mktime(&loc));

    exit(EXIT_SUCCESS);
    
}