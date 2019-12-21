#ifndef _ISSHE_SIGNAL_H_
#define _ISSHE_SIGNAL_H_

#include <signal.h>

/*
typedef void (* sigfunc)(int)
*/
typedef void sigfunc(int);

/*
 * 出错不退出
 */
sigfunc *signal(int signo, sigfunc *handler);

/*
 * 出错退出
 */
sigfunc *isshe_signal(int signo, sigfunc *handler);

#endif