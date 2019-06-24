#include "isshe_signal.h"
#include "isshe_error.h"
#include "isshe_common.h"

/**
 * 可移植的signal函数：
 *  1.只有这个处理程序当前正在处理的那种类型的信号被阻塞。
 *  2.和所有信号实现一样，信号不会排队等待。
 *  3.只要可能，被中断的系统调用会自动重启。
 *  4. 一旦设置了信号处理程序，它就会一直保持，
 *      直到此函数带着handler参数为SIG_IGN或者SIG_DFL被调用。
 */
sigfunc *signal(int signo, sigfunc *handler)
{
    struct sigaction action;
    struct sigaction old_action;

    action.sa_handler = handler;
    // Block sigs of type being handled（阻塞被捕获的信号）
    sigemptyset(&action.sa_mask);   // 清空sa_mask，执行信号处理函数期间，不阻塞任何信号
    action.sa_flags = 0;

    if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
        action.sa_flags |= SA_INTERRUPT;    /* SunOS 4.x */
#endif
    } else {
#ifdef  SA_RESTART
        //Restart syscalls if possible（尽可能重启系统调用）
        action.sa_flags |= SA_RESTART;         /* SVR4, 44BSD */
#endif
    }

    if (sigaction(signo, &action, &old_action) < ISSHE_SUCCESS) {
        return SIG_ERR;
    }

    return (old_action.sa_handler);
}

sigfunc *isshe_signal(int signo, sigfunc *handler)
{
    sigfunc *sfunc;

    if ( (sfunc = signal(signo, handler)) == SIG_ERR) {
        isshe_sys_error_exit("signal error");
    }

    return(sfunc);
}