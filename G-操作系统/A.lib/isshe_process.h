#ifndef _ISSHE_PROCESS_H_
#define _ISSHE_PROCESS_H_

#include <unistd.h>

pid_t isshe_fork(void);
void print_exit_status(int status);

#endif