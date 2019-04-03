#include "isshe_signal.h"
#include "isshe_common.h"

/**
 * 可移植的signal函数：
 *  1.只有这个处理程序当前正在处理得哪种类型的信号被阻塞。
 *  2.和所有信号实现一样，信号不会排队等待。
 *  3.只要可能，被中断的系统调用会自动重启。
 *  4. 一旦设置了信号处理程序，它就会一直保持，
 *      直到此函数带着handler参数为SIG_IGN或者SIG_DFL被调用。
 */
sighandler_t isshe_signal(int signum, sighandler_t handler)
{
    struct sigaction action;
    struct sigaction old_action;

    action.sa_handler = handler;
    // Block sigs of type being handled（阻塞被捕获的信号）
    sigemptyset(&action.sa_mask);
    //Restart syscalls if possible（尽可能重启系统调用）
    action.sa_flags = SA_RESETHAND;

    if (sigaction(signum, &action, &old_action) < 0) {
        isshe_error("isshe_signal error");
    }

    return (old_action.sa_handler);
}