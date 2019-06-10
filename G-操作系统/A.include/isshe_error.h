#ifndef _ISSHE_ERROR_H_
#define _ISSHE_ERROR_H_

#define ISSHE_ERROR_MAX_LINE    4096

enum error_type {
    UNIX_ERROR = 0,
    POSIX_ERROR,
    GAI_ERROR,
    APP_ERROR,
    DNS_ERROR,
};

/*
 * 系统调用错误；不退出；
 */
void isshe_sys_error(const char *fmt, ...);

/*
 * 系统调用错误；退出；core dump；
 */
void isshe_sys_error_dump(const char *fmt, ...);

/*
 * 系统调用错误；不退出；
 */
void isshe_sys_error_exit(const char *fmt, ...);

/*
 * 用户调用错误；不退出；
 */
void isshe_error(const char *fmt, ...);

/*
 * 用户调用错误；退出；
 */
void isshe_error_exit(const char *fmt, ...);

/*
 * 系统调用错误；INFO级别，不退出
 */
void isshe_sys_info(const char *fmt, ...);

/*
 * 用户调用错误；INFO级别，不退出
 */
void isshe_info(const char *fmt, ...);

/*
 * 和isshe_sys_error()相同
 */
void isshe_unix_error(const char *fmt, ...);

/*
 * 和isshe_sys_error_exit()相同
 */
void isshe_unix_error_exit(const char *fmt, ...);

void isshe_posix_error(int code, const char *fmt, ...);

void isshe_posix_error_exit(int code, const char *fmt, ...);

void isshe_gai_error(int code, const char *fmt, ...);

void isshe_gai_error_exit(int code, const char *fmt, ...);

/*
 * 和isshe_error()相同
 */
void isshe_app_error(const char *fmt, ...);

/*
 * 和isshe_error_exit()相同
 */
void isshe_app_error_exit(const char *fmt, ...);

#endif