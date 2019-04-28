

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
    int     readfifo, writefifo;
    char    *ptr, buf[ISSHE_MAXLINE];
    char    fifoname[ISSHE_MAXLINE];
    pid_t   pid;
    ssize_t n;
    size_t	len;

    // 创建fifo
    pid = getpid();
    snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", (long)pid);
    if ((mkfifo(fifoname, ISSHE_FILE_MODE) < 0) && (errno != EEXIST)) {
        isshe_sys_error_exit("can't create %s", SERV_FIFO);
    }

    snprintf(buf, sizeof(buf), "%ld ", (long)pid);
    len = strlen(buf);
    ptr = buf + len;

    // 读路径
    isshe_fgets(ptr, ISSHE_MAXLINE - len, stdin);
    len = strlen(buf);

    // 打开服务器的fifo
    writefifo = isshe_open(SERV_FIFO, O_WRONLY, 0);
    isshe_write(writefifo, buf, len);

    readfifo = isshe_open(fifoname, O_RDONLY, 0);

    while((n = isshe_read(readfifo, buf, ISSHE_MAXLINE)) > 0) {
        isshe_write(STDOUT_FILENO, buf, n);
    }

    isshe_close(readfifo);
    isshe_unlink(fifoname);
    exit(0);
}