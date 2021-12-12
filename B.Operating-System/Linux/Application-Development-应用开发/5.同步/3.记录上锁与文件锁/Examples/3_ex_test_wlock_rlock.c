// 此程序测试：在持有读锁并有写锁等待的情况下，其他读锁是否被允许。

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "isshe_common.h"
#include "isshe_process.h"
#include "isshe_file.h"
#include "isshe_unistd.h"
#include "isshe_time.h"

int main(int argc, char **argv)
{
    int fd;

    fd = isshe_open("test1.temp", O_RDWR | O_CREAT, ISSHE_FILE_MODE);
    printf("fd = %d\n", fd);
    isshe_read_lock(fd, 0, SEEK_SET, 0);  // 父进程加读锁
    printf("fd = %d\n", fd);
    printf("%s: 父进程加了读锁\n", isshe_gf_time());

    if (isshe_fork() == 0) {
        // 第一个子进程
        sleep(1);
        printf("%s: 第一个子进程尝试获取写锁\n", isshe_gf_time());
        isshe_writew_lock(fd, 0, SEEK_SET, 0);	// 这里至少等5s
        printf("%s: 第一个子进程获取到写锁\n", isshe_gf_time());
        sleep(2);
        isshe_un_lock(fd, 0, SEEK_SET, 0);
        printf("%s: 第一个子进程释放写锁\n", isshe_gf_time());
        exit(0);
    }

    if (isshe_fork() == 0) {
        // 第二个子进程
        sleep(3);
        printf("%s: 第二个进程尝试获取读锁\n", isshe_gf_time());
        isshe_readw_lock(fd, 0, SEEK_SET, 0);
        printf("%s: 第二个进程获得读锁\n", isshe_gf_time());
        sleep(4);
        isshe_un_lock(fd, 0, SEEK_SET, 0);
        printf("%s: 第二个进程释放读锁\n", isshe_gf_time());
        exit(0);
    }

    sleep(5);
    isshe_un_lock(fd, 0, SEEK_SET, 0);
    printf("%s: 父进程释放读锁\n", isshe_gf_time());
    exit(0);
}