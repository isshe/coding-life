
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "isshe_process.h"
#include "isshe_signal.h"

#define MAXBUF 1024

/**
 * 此程序有缺陷：
 *  1. 无法处理信号阻塞
 *  2. 信号不排队等待
 *  3. 系统调用被中断 
 * 教训：不可以用信号来对其他进程中发生的时间计数！
 */
void handler1(int sig)
{
    pid_t pid;

    if ((pid = waitpid(-1, NULL, 0)) < 0) {
        printf("waitpid error\n");
        exit(0);
    }
    printf("Handler reaped child %d\n", (int)pid);
    sleep(2);
    return;
}

/**
 * 修复handler1的2.信号不排队等待
 */
void handler2(int sig)
{
    pid_t pid;

    while ((pid = waitpid(-1, NULL, 0)) > 0) {
        printf("Handler reaped child %d\n", (int)pid);
    }

    if (errno != ECHILD) {
        printf("waitpid error\n");
        exit(0);
    }

    sleep(2);
    return;
}

int main(void) {
    int i, n;
    char buf[MAXBUF];
    
    //if (isshe_signal(SIGCHLD, handler2) == SIG_ERR) {
    if (signal(SIGCHLD, handler2) == SIG_ERR) {
        printf("signal error\n");
        exit(0);
    }
    for (i = 0; i < 3; i++) {
        if (isshe_fork() == 0) {
            printf("Hello from child %d\n", (int)getpid());
            sleep(1);
            exit(0);
        }
    }

    /* 中断后不能恢复
    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        printf("read error!\n");
        exit(0);
    }
    */

    // 解决被信号中断后无法恢复的问题
    while((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0) {
        if (errno != EINTR) {
            printf("read error\n");
            exit(0);
        } // else error == EINTR, continue next loop
    }
    printf("Parent processing input\n");

    while(1) {
        ;
    }

    exit(0);
}