#ifndef _ISSHE_PROCESS_H_
#define _ISSHE_PROCESS_H_

#include <unistd.h>

pid_t isshe_fork(void);
void isshe_print_exit_status(int status);

/**
 * daemonize: 守护进程化——使一个进程"变为"守护进程
 */
void daemonize(const char *cmd);

#endif