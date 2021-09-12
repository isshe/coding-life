
#include <stdio.h>

#include "isshe_stdio.h"
#include "isshe_error.h"


char *isshe_fgets(char *ptr, int n, FILE *stream)
{
    char    *rptr;

    if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
        isshe_sys_error_exit("fgets error");

    return (rptr);
}

void isshe_fputs(const char *ptr, FILE *stream)
{
    if (fputs(ptr, stream) == EOF) {
        isshe_sys_error_exit("fputs error");
    }
}