#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>

#include "../../A.lib/isshe_process.h"
#include "../../A.lib/isshe_common.h"


void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    // 清除文件模式创建屏蔽字
    umask(0);

    // 获取最大文件描述符
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        printf("%s: can't get file limit\n", cmd);
        exit(0);
    }

    // 成为会话leader，没有控制终端
    pid = isshe_fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(0);
    } else if (pid != 0) {
        // parent
        exit(0);
    }

    setsid();

    // 确保之后的open不会分配控制终端
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        printf("%s: can't ignore SIGHUP\n", cmd);
        exit(0);
    }
    pid = isshe_fork();
    if (pid != 0) {
        // parent
        exit(0);
    }

    // 改工作目录
    if (chdir("/") < 0) {
        printf("%s: can't change directory to /", cmd);
        exit(0);
    }

    // close all open file descriptors
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }
    for (i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    // 把0、1、2整到/dev/null
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    // 初始化log文件
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d\n", fd0, fd1, fd2);
        exit(1);
    }
    syslog(LOG_ERR, "-----isshe-----daemonize run finish\n");
}

// mac os 下，打开console.app进行观察（设备栏）, 可看到log
// ps -ejf | grep -E 'PPID|a.out' 运行程序10s内，运行此命令，可看到进程
int main(int argc, char *argv[]) {
    printf("argv[0] = %s", argv[0]);
    daemonize(argv[0]);
    sleep(10);
    return 0;
}