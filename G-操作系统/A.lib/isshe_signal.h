#ifndef _ISSHE_SIGNAL_H_
#define _ISSHE_SIGNAL_H_

#include <signal.h>
typedef void (*sighandler_t)(int);

sighandler_t isshe_signal(int signum, sighandler_t handler);

#endif