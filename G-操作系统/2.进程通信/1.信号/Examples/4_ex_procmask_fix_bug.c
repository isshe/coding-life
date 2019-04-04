

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../../../A.lib/isshe_process.h"
#include "../../../A.lib/isshe_signal.h"


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
    sigset_t mask;

    if (isshe_signal(SIGCHLD, handler) == SIG_ERR) {
        printf("signal error\n");
        exit(0);
    }
    initjobs();

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);      // 添加SIGCHLD到信号集中
    sigprocmask(SIG_BLOCK, &mask, NULL);    // 把SIGCHLD添加到阻塞列表中

    if ((pid = isshe_fork()) == 0) {
        sigprocmask(SIG_UNBLOCK, &mask, NULL);  // 子进程解除对SIGCHLD的阻塞
        execve("/bin/date", argv, NULL);
    }

    addjob(pid);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);  // 父进程解除对SIGCHLD的阻塞

    while(1) {
        ;
    }

    exit(0);
}