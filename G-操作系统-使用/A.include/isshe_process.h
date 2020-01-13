#ifndef _ISSHE_PROCESS_H_
#define _ISSHE_PROCESS_H_

#include "isshe_common.h"

#define isshe_log_pid            getpid()

pid_t isshe_fork(void);
void isshe_print_exit_status(int status);

/**
 * daemonize: 守护进程化——使一个进程"变为"守护进程
 */
void isshe_daemonize(const char *pname, int facility);

pid_t isshe_waitpid(pid_t pid, int *iptr, int options);

#endif