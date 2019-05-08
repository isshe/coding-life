
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>

#ifdef __linux__
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include "isshe_file.h"
#include "isshe_common.h"
#include "isshe_process.h"

#define LOCKFILE "/tmp/isshe_daemon.pid"

static void book_daemonize(const char *cmd)
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
    syslog(LOG_ERR, "-----isshe-----book_daemonize run finish\n");
}


int isshe_already_running(const char *lock_file)
{
    int fd;
    char buf[16];

    fd = open(lock_file, O_RDWR|O_CREAT, LOCKMODE);
    if (fd < 0) {
        // syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        printf("can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }

    if (isshe_lock_file(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return ISSHE_TRUE;
        }
        // syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        printf("can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    //isshe_unlock_file(fd);
    //close(fd);    // 如果关闭，会释放锁

    return ISSHE_FALSE;
}

void reread(void)
{
    syslog(LOG_ERR, "reread...");
    printf("reread...");
}

void sigterm(int signo)
{
    syslog(LOG_ERR, "got SIGTERM; exiting...");
    syslog(LOG_INFO, "got SIGTERM; exiting...");
    exit(0);
}

void sighup(int signo)
{
    syslog(LOG_ERR, "got SIGHUP; Re-reading...");
    syslog(LOG_INFO, "got SIGHUP; Re-reading...");
    reread();
}

// SIGHUP: 1 = 0x1
// SIGTERM: 15 = 0xf
// kill -x pid
int main(int argc, char *argv[])
{
    char *cmd;
    struct sigaction sa;

    if ((cmd = strrchr(argv[0], '/')) == NULL) {
        cmd = argv[0];
    } else {
        cmd++;
    }

    // 成为守护进程
    book_daemonize(cmd);

    // 确保只有一个守护进程
    if (isshe_already_running(LOCKFILE)) {
        syslog(LOG_ERR, "daemon already running...\n");
        exit(1);
    }

    sa.sa_handler = sigterm;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGHUP); // 当调用sigterm时，屏蔽SIGHUP信号
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) < 0) {
        syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
        exit(1);
    }

    sa.sa_handler = sighup;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM); // 当调用sighup时，屏蔽SIGTERM信号
    sa.sa_flags = 0; //SA_RESTART 无法重启sleep（大概是因为：sleep是标准库函数，不是系统调用）
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
        exit(1);
    }

    syslog(LOG_ERR, "sleep...pid = %d", getpid());
    sleep(30);
    syslog(LOG_ERR, "sleep done!");
    exit(0);
}

