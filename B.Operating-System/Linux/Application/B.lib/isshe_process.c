#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/wait.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#ifdef __linux__
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include "isshe_process.h"
#include "isshe_error.h"
#include "isshe_common.h"
#include "isshe_signal.h"

extern int daemon_proc;    // defined in isshe_error.c

pid_t isshe_fork(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        isshe_sys_error_exit("Fork error");
    }

    return pid;
}

void isshe_print_exit_status(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
        WCOREDUMP(status) ? "(core file generated)" : "");
#else
        "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}

void isshe_daemonize(const char *pname, int facility)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;

    // 清除文件模式创建屏蔽字
    umask(0);

    // 获取最大文件描述符
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        printf("%s: can't get file limit\n", pname);
        exit(0);
    }

    // 成为会话leader，没有控制终端
    pid = isshe_fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(0);
    } else if (pid != 0) {
        exit(0);    // 终止父进程
    }

    setsid();

    // 确保之后的open不会分配控制终端
    isshe_signal(SIGHUP, SIG_IGN);

    pid = isshe_fork();
    if (pid != 0) {
        exit(0);    // 终止第一个子进程
    }

    daemon_proc = 1;    /* for our err_XXX() functions */

    // 改工作目录
    if (chdir("/") < 0) {
        printf("%s: can't change directory to /", pname);
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
    openlog(pname, LOG_PID, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d\n", fd0, fd1, fd2);
        exit(1);
    }
}

pid_t isshe_waitpid(pid_t pid, int *iptr, int options)
{
    pid_t ret_pid;

    if ( (ret_pid = waitpid(pid, iptr, options)) == ISSHE_FAILURE ) {
        isshe_sys_error_exit("waitpid error");
    }

    return ret_pid;
}
