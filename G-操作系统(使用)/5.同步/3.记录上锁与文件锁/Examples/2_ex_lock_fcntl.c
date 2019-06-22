#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "isshe_common.h"
#include "isshe_file.h"
#include "isshe_unistd.h"

#define SEQFILE "seqno"     // filename

void my_lock(int fd)
{
    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    isshe_fcntl(fd, F_SETLKW, &lock);
}

void my_unlock(int fd)
{
    struct flock lock;

    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    isshe_fcntl(fd, F_SETLK, &lock);
}

int main(int argc, char *argv[])
{
    int fd;
    long i, seqno;
    pid_t pid;
    ssize_t n;
    char line[ISSHE_MAXLINE + 1];

    pid = getpid();
    fd = isshe_open(SEQFILE, O_RDWR, ISSHE_FILE_MODE);

    for (i = 0; i < 20; i++) {
        my_lock(fd);
        // sleep(10);       // 锁期间其他文件能修改文件内容。（注意：使用强制性锁也还是有问题！）

        isshe_lseek( fd, 0L, SEEK_SET);
        n = isshe_read(fd, line, ISSHE_MAXLINE);
        line[n] = '\0';

        n = sscanf(line, "%ld\n", &seqno);
        printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);

        seqno++;

        snprintf(line, sizeof(line), "%ld\n", seqno);
        isshe_lseek(fd, 0L, SEEK_SET);
        isshe_write(fd, line, strlen(line));

        my_unlock(fd);
    }

    exit(0);
}