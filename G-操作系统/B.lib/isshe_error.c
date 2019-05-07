#include <stdio.h>
#include <errno.h>      // for "errno"
#include <stdarg.h>     // for "可变参"
#include <string.h>
#include <stdlib.h>

#include "isshe_error.h"

/*
 * 打印一个信息，并返回到调用者。
 * 调用者指定"errno_flag"。
 * 改变这个函数，就能实现打印到文件中或者其他地方。
 */
static void
err_doit(enum caller_type type, int error, const char *fmt, va_list ap)
{
    char    buf[ISSHE_ERROR_MAX_LINE];

    vsnprintf(buf, ISSHE_ERROR_MAX_LINE-1, fmt, ap);
    if (type == SYSTEM_CALL) {
        snprintf(buf+strlen(buf), ISSHE_ERROR_MAX_LINE-strlen(buf)-1, ": %s", strerror(error));
    }

    strcat(buf, "\n");
    fflush(stdout);     // in case stdout and stderr are the same
    fputs(buf, stderr);
    fflush(NULL);       // flushes all stdio output streams
}

void isshe_sys_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, errno, fmt, ap);
    va_end(ap);
}

void isshe_sys_error_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, errno, fmt, ap);
    va_end(ap);
    abort();        // dump core and terminate
    exit(1);        // shouldn't get here
}

void isshe_sys_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, errno, fmt, ap);
    va_end(ap);
    exit(1);
}

void isshe_sys_error_param(int error, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, error, fmt, ap);
    va_end(ap);
}

void isshe_sys_error_param_exit(int error, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, error, fmt, ap);
    va_end(ap);
    exit(1);
}

void isshe_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(USER_CALL, 0, fmt, ap);
    va_end(ap);
}

void isshe_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(USER_CALL, 0, fmt, ap);
    va_end(ap);
    exit(1);
}
