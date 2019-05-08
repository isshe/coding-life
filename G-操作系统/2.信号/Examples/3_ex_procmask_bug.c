
/*
这个程序的问题在于：（异常场景）
1.父进程创建子进程后，子进程先运行；(如果父进程先运行，这里就没问题啦，不过哪个先运行是未定义/未规定的！况且，多核情况下，可能一起运行！)
2.子进程运行终止，变为僵尸进程，使得内核传递一个SIGCHLD信号给父进程；
3.父进程再次运行之前，内核检查发现有一个SIGCHLD信号，通过在父进程中运行信号处理程序接收信号；
4.信号处理程序回收子进程，并调用deletejob（还没addjob呢此时）
5.信号处理程序执行完毕，父进程继续到fork之后执行，调用addjob，把已经结束的子进程添加到管理列表中。
*/

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include "isshe_process.h"
#include "isshe_signal.h"

void initjobs()
{
}

void addjob(int pid)
{
}

void deletejob(int pid)
{

}

/**
 * 修复handler1的2.信号不排队等待
 */
void handler(int sig)
{
    pid_t pid;

    while ((pid = waitpid(-1, NULL, 0)) > 0) {
        deletejob(pid);
    }

    if (errno != ECHILD) {
        printf("waitpid error\n");
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;

    if (isshe_signal(SIGCHLD, handler) == SIG_ERR) {
        printf("signal error\n");
        exit(0);
    }

    initjobs();
    if ((pid = isshe_fork()) == 0) {
        execve("/bin/date", argv, NULL);
    }

    addjob(pid);

    while(1) {
        ;
    }

    exit(0);
}