#ifndef _ISSHE_ERROR_H_
#define _ISSHE_ERROR_H_

#define ISSHE_ERROR_MAX_LINE    4096

enum caller_type{
    USER_CALL = 0,
    SYSTEM_CALL = 1
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
 * 系统调用错误；不退出；错误码作为参数
 */
void isshe_sys_error_param(int error, const char *fmt, ...);

/*
 * 系统调用错误；退出；错误码作为参数
 */
void isshe_sys_error_param_exit(int error, const char *fmt, ...);

/*
 * 用户调用错误；不退出；
 */
void isshe_error(const char *fmt, ...);

/*
 * 用户调用错误；退出；
 */
void isshe_error_exit(const char *fmt, ...);

#endif