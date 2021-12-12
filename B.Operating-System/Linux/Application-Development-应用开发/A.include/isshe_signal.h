#ifndef _ISSHE_SIGNAL_H_
#define _ISSHE_SIGNAL_H_

#include "isshe_common.h"

#define ISSHE_SIGNAL_ERROR          SIG_ERR

#define ISSHE_SIGNAL_RECONFIGURE    SIGHUP
#define ISSHE_SIGNAL_SHUTDOWN       SIGQUIT
#define ISSHE_SIGNAL_TERMINATE      SIGTERM
#define ISSHE_SIGNAL_REOPEN         SIGUSR1
#define ISSHE_SIGNAL_ALARM          SIGALRM
#define ISSHE_SIGNAL_INTERRUPT      SIGINT
#define ISSHE_SIGNAL_IO             SIGIO
#define ISSHE_SIGNAL_CHILD          SIGCHLD
#define ISSHE_SIGNAL_SYSTEM         SIGSYS
#define ISSHE_SIGNAL_PIPE           SIGPIPE
#define ISSHE_SIGNAL_KILL           SIGKILL

//#define ISSHE_NOACCEPT_SIGNAL      SIGWINCH
//#define ISSHE_CHANGEBIN_SIGNAL     SIGUSR2

#define isshe_kill(pid, signo)      (isshe_int_t)kill(pid, signo)
#define isshe_signal_send           isshe_kill

typedef void (*isshe_signal_handler_t)(int);
typedef struct isshe_signal_s isshe_signal_t;

struct isshe_signal_s{
    isshe_int_t     signo;
    isshe_char_t    *signame;
    isshe_char_t    *name;
    isshe_signal_handler_t handler;
};

/*
 * 出错不退出
 */
isshe_signal_handler_t isshe_sigaction(isshe_int_t signo, 
    isshe_signal_handler_t handler);

/*
 * 出错退出
 */
isshe_signal_handler_t isshe_signal(isshe_int_t signo, 
    isshe_signal_handler_t handler);

#endif