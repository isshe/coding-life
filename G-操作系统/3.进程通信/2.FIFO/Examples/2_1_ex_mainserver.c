

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "isshe_file.h"
#include "isshe_error.h"
#include "isshe_stdio.h"
#include "isshe_common.h"

#define	SERV_FIFO "/tmp/fifo.serv"

int main(int argc, char *argv[])
{
    int readfifo, writefifo, dummyfd, clientfd;
    char *ptr, buf[ISSHE_MAXLINE + 1];
    char fifoname[ISSHE_MAXLINE];
    pid_t pid;
    ssize_t n;

    // 创建fifo
    if ((mkfifo(SERV_FIFO, ISSHE_FILE_MODE) < 0) && (errno != EEXIST)) {
        isshe_sys_error_exit("can't create %s", SERV_FIFO);
    }

    readfifo = isshe_open(SERV_FIFO, O_RDONLY, 0);
    dummyfd = isshe_open(SERV_FIFO, O_WRONLY, 0);   // 不用，但是开着！

    while((n = isshe_readline(readfifo, buf, ISSHE_MAXLINE)) > 0) {
        if (buf[n - 1] == '\n') {
            n--;
        }
        buf[n] = '\0';

        // "PID pathname"
        if ((ptr = strchr(buf, ' ')) == NULL) {
            isshe_sys_error("bogus request: %s", buf);
            continue;
        }
        *ptr++ = 0; // 分割PID和内容

        // 取得客户进程的fifo名称
        pid = atol(buf);
        snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", (long)pid);
        if ( (writefifo = open(fifoname, O_WRONLY, 0)) < 0 ) {
            isshe_sys_error("cannot open: %s", fifoname);
            continue;
        }

        if ( (clientfd = open(ptr, O_RDONLY)) < 0 ) {
            // 告诉客户端出错了
            snprintf(buf + n, sizeof(buf) - n, ": cannot open: %s\n", strerror(errno));
            n = strlen(ptr);
            isshe_write(writefifo, ptr, n);
            isshe_close(writefifo);
        } else {
            // 打开成功，复制文件内容到fifo
            while( (n = isshe_read(clientfd, buf, ISSHE_MAXLINE)) > 0 ) {
                isshe_write(writefifo, buf, n);
            }
            isshe_close(clientfd);
            isshe_close(writefifo);
        }
    }

    exit(0);
}