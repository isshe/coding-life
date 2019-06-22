#ifndef _ISSHE_SIGNAL_H_
#define _ISSHE_SIGNAL_H_

#include <signal.h>

/*
typedef void (* sighandler_t)(int)
*/
typedef void sighandler_t(int);

/*
 * 出错不退出
 */
sighandler_t *signal(int signo, sighandler_t *handler);

/*
 * 出错退出
 */
sighandler_t *isshe_signal(int signo, sighandler_t *handler);

#endif