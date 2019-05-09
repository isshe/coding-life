#include <sys/time.h>
#include <string.h>
#include <stdio.h>

#include "isshe_time.h"
#include "isshe_error.h"

char *isshe_gf_time(void)
{
    struct timeval	tv;
    static char		str[30];
    char			*ptr;

    if (gettimeofday(&tv, NULL) < 0) {
        isshe_sys_error_exit("gettimeofday error");
    }

    ptr = ctime(&tv.tv_sec);
    strcpy(str, &ptr[11]);
        /* Fri Sep 13 00:00:00 1986\n\0 */
        /* 0123456789012345678901234 5  */
    snprintf(str+8, sizeof(str)-8, ".%06ld", (long)tv.tv_usec);

    return(str);
}