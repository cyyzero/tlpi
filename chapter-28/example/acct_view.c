#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/acct.h>
#include <limits.h>
#include "ugid_functions.h"
#include "tlpi_hdr.h"

#define TIME_BUF_SIZE 100

static long long compt_to_LL(compt_t ct)
{
    const int EXP_SIZE = 3;
    const int MANTISSA_SIZE = 13;
    const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
    long long mantissa, exp;

    mantissa = ct & MANTISSA_MASK;
    exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1);
    return mantissa << (exp * 3);
}

int main(int argc, char *argv[])
{
    int acct_file;
    struct acct ac;
    ssize_t num_read;
    char *s;
    char time_buf[TIME_BUF_SIZE];
    struct tm *loc;
    time_t t;

    acct_file = open(argv[1], O_RDONLY);
    if (acct_file == -1)
        errExit("open");

    printf("command flags   term.  user    "
           "start time            CPU  elapsed\n");
    printf("               status          "
           "                      time   time\n");

    while ((num_read = read(acct_file, &ac, sizeof(struct acct))) > 0)
    {
        if (num_read != sizeof(struct acct))
            fatal("partial read");

        printf("%-8.8s  ", ac.ac_comm);
        printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-');
        printf("%c", (ac.ac_flag & ASU  ) ? 'S' : '-');
        printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-');
        printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-');

#ifdef __linux__
        printf(" %#6lx  ", (unsigned long)ac.ac_exitcode);
#else
        printf(" %#6lx  ", (unsigned long)ac.ac_stat);
#endif
        s = user_name_from_id(ac.ac_uid);
        printf("%-8.8s ", (s == NULL) ? "???" : s);

        t = ac.ac_btime;
        loc = localtime(&t);
        if (loc == NULL)
        {
            printf("???Unknown time???  ");
        }
        else
        {
            strftime(time_buf, TIME_BUF_SIZE, "%Y-%m-%d %T", loc);
            printf("%s", time_buf);
        }

        printf("%5.2f %7.2f ", (double) (comptToLL(ac.ac_utime) +
                comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK),
                (double) comptToLL(ac.ac_etime) / sysconf(_SC_CLK_TCK));
        printf("\n");
    }
    if (num_read == -1)
        errExit("read");
    exit(EXIT_SUCCESS);
}