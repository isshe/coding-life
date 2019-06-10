#include <stdio.h>
#include <errno.h>      // for "errno"
#include <stdarg.h>     // for "可变参"
#include <string.h>
#include <stdlib.h>
#include <syslog.h>     // for syslog()
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>  // for inet_ntop()/inet_pton()

#include "isshe_error.h"

int     daemon_proc;    /* 被daemon_init()设置为非0，表示守护进程 */

/*
 * 打印一个信息，并返回到调用者。
 * 调用者指定"errno_flag"。
 * 改变这个函数，就能实现打印到文件中或者其他地方。
 */
static void err_doit(enum error_type type,
    int error_code, int level, const char *fmt, va_list ap)
{
    int     n;
    char    buf[ISSHE_ERROR_MAX_LINE];

    vsnprintf(buf, sizeof(buf), fmt, ap);

    n = strlen(buf);
    if (type == UNIX_ERROR || type == POSIX_ERROR ) {
        snprintf(buf+n, sizeof(buf)-n, ": %s", strerror(error_code));
    } else if (type == GAI_ERROR) {
        snprintf(buf+n, sizeof(buf)-n, ": %s", gai_strerror(error_code));
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
    err_doit(UNIX_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_sys_error_dump(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(UNIX_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
    abort();        // dump core and terminate
    exit(0);        // shouldn't get here
}

void isshe_sys_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(UNIX_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(0);
}

void isshe_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(APP_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(APP_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(0);
}

void isshe_sys_info(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(UNIX_ERROR, errno, LOG_INFO, fmt, ap);
    va_end(ap);
}

void isshe_info(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(APP_ERROR, errno, LOG_INFO, fmt, ap);
    va_end(ap);
}

void isshe_unix_error(const char *fmt, ...) /* Unix-style error */
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(UNIX_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_unix_error_exit(const char *fmt, ...) /* Unix-style error */
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(UNIX_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(0);
}

void isshe_posix_error(int code, const char *fmt, ...) /* Posix-style error */
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(POSIX_ERROR, code, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_posix_error_exit(int code, const char *fmt, ...) /* Posix-style error */
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(POSIX_ERROR, code, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(0);
}

void isshe_gai_error(int code, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(GAI_ERROR, code, LOG_ERR, fmt, ap);
    va_end(ap);
}


void isshe_gai_error_exit(int code, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(GAI_ERROR, code, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(0);
}

void isshe_app_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(APP_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
}

void isshe_app_error_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    err_doit(APP_ERROR, errno, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(0);
}