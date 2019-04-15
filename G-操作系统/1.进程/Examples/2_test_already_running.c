
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "isshe_file.h"
#include "isshe_common.h"


#define LOCKFILE "/tmp/isshe_daemon.pid"

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


int main(void)
{
    if (isshe_already_running(LOCKFILE)) {
        printf("is running...\n");
    } else {
        sleep(30);
    }

    // 退出后，会关闭fd，并释放相关锁
    return 0;
}

