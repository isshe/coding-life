
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "isshe_file.h"
#include "isshe_error.h"
#include "isshe_common.h"

static int isshe_lock_unlock(int fd, short type) {
    struct flock fl;

    fl.l_type = type;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;

    return(fcntl(fd, F_SETLK, &fl));
}

int isshe_lock_file(int fd)
{
    return isshe_lock_unlock(fd, F_WRLCK);
}


int isshe_unlock_file(int fd)
{
    return isshe_lock_unlock(fd, F_UNLCK);
}

int isshe_open(const char *pathname, int oflag, ...)
{
    int fd;
    va_list ap;
    mode_t mode;

    if (oflag & O_CREAT) {
        va_start(ap, oflag);
        mode = va_arg(ap, va_mode_t);
        if ( (fd = open(pathname, oflag, mode)) == ISSHE_FAILED ) {
            isshe_error_exit("open error for %s", pathname);
        }
        va_end(ap);
    } else {
        if ( (fd = open(pathname, oflag)) == ISSHE_FAILED ) {
            isshe_error_exit("open error for %s", pathname);
        }
    }

    return fd;
}

void isshe_close(int fd)
{
    if (close(fd) == ISSHE_FAILED) {
        isshe_error_exit("close error");
    }
}

void isshe_unlink(const char *pathname)
{
    if (unlink(pathname) == ISSHE_FAILED) {
        isshe_error_exit("unlink error for %s", pathname);
    }
}