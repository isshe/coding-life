#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include "isshe_error.h"

#define MAXLINE 1024
#define DEF_PAGER "/usr/bin/more"   // macOS pager program

int main(int argc, char *argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    char *pager, *argv0;
    FILE *fp;

    if (argc != 2) {
        isshe_error_exit("Usage: a.out <pathname>");
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        isshe_error_exit("can't open %s", argv[1]);
    }

    if (pipe(fd) < 0) {
        isshe_error_exit("pipe error");
    }

    if ((pid = fork()) < 0) {
        isshe_error_exit("fork error");
    } else if (pid > 0) {
        // parent
        close(fd[0]);

        // 父进程复制argv[1]到pipe
        while (fgets(line, MAXLINE, fp) != NULL) {
            n = strlen(line);
            if (write(fd[1], line, n) != n) {
                isshe_sys_error_exit("write error to pipe");
            }
        }

        if (ferror(fp)) {
            isshe_sys_error_exit("fgets error");
        }
        close(fd[1]);   // 关闭后，读完会返回0
        if (waitpid(pid, NULL, 0) < 0) {
            isshe_sys_error_exit("waitpid error");
        }
    } else {
        close(fd[1]);   // 关闭写文件描述符
        if (fd[0] != STDIN_FILENO) {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
                isshe_sys_error_exit("dup2 error to stdin");
            }
            close(fd[0]);
        }

        // 获取execl()需要的参数
        //pager = getenv("PAGER");
        //if (pager == NULL) {
            pager = DEF_PAGER;
        //}

        argv0 = strrchr(pager, '/');
        if (argv0 != NULL) {
            argv0++;
        } else {
            argv0 = pager;
        }

        if (execl(pager, argv0, (char *)0) < 0) {
            isshe_sys_error_exit("execl error for %s", pager);
        }
    }

    exit(0);
}