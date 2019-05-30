#include <stdio.h>
#include <errno.h>      // for "errno"
#include <stdarg.h>     // for "可变参"
#include <string.h>
#include <stdlib.h>
#include <syslog.h>     // for syslog()

#include "isshe_error.h"

int     daemon_proc;    /* set nonzero by daemon_init() */
/*
 * 打印一个信息，并返回到调用者。
 * 调用者指定"errno_flag"。
 * 改变这个函数，就能实现打印到文件中或者其他地方。
 */
static void
err_doit(enum caller_type type, int level, const char *fmt, va_list ap)
{
    int     errno_save, n;
    char    buf[ISSHE_ERROR_MAX_LINE];

    errno_save = errno;     /* value caller might want printed */ //isshe???
    vsnprintf(buf, sizeof(buf), fmt, ap);

    n = strlen(buf);
    if (type == SYSTEM_CALL) {
        snprintf(buf+n, sizeof(buf)-n, ": %s", strerror(errno_save));
    }
    strcat(buf, "\n");

    if (daemon_proc) {
        syslog(level, "%s", buf);
    } else {
        fflush(stdout);     // in case stdout and stderr are the same
        fputs(buf, stderr);
        fflush(NULL);       // flushes all stdio output streams
    }

    return;
}

void isshe_sys_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_sys_error_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();        // dump core and terminate
    exit(1);        // shouldn't get here
}

void isshe_sys_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

void isshe_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(USER_CALL, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(USER_CALL, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

void isshe_sys_info(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(SYSTEM_CALL, LOG_INFO, fmt, ap);
    va_end(ap);
}

void isshe_info(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(USER_CALL, LOG_INFO, fmt, ap);
    va_end(ap);
}
