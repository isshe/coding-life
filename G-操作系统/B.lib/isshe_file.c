
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>

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
            isshe_sys_error_exit("open error for %s", pathname);
        }
        va_end(ap);
    } else {
        if ( (fd = open(pathname, oflag)) == ISSHE_FAILED ) {
            isshe_sys_error_exit("open error for %s", pathname);
        }
    }

    return fd;
}

void isshe_close(int fd)
{
    if (close(fd) == ISSHE_FAILED) {
        isshe_sys_error_exit("close error");
    }
}

ssize_t isshe_read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if ( (n = read(fd, ptr, nbytes)) == -1) {
        isshe_sys_error_exit("read error");
    }

    return(n);
}

void isshe_write(int fd, void *ptr, size_t nbytes)
{
    if (write(fd, ptr, nbytes) != nbytes){
        isshe_sys_error_exit("write error");
    }
}

void isshe_unlink(const char *pathname)
{
    if (unlink(pathname) == ISSHE_FAILED) {
        isshe_sys_error_exit("unlink error for %s", pathname);
    }
}

static ssize_t restart_read(int fd, char *ptr)
{
    static int	read_cnt = 0;
    static char	*read_ptr;
    static char	read_buf[ISSHE_MAXLINE];

    if (read_cnt <= 0) {
        while(1) {
            if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
                if (errno == EINTR) {
                    continue;
                }
                return(-1);
            } else if (read_cnt == 0) {
                return(0);
            }
            read_ptr = read_buf;
            break;
        }
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return(1);
}

static ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    int n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ( (rc = restart_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;      /* newline is stored, like fgets() */
        } else if (rc == 0) {
            if (n == 1)
                return(0);  /* EOF, no data read */
            else
                break;      /* EOF, some data was read */
        } else
            return(-1);     /* error, errno set by read() */
    }

    *ptr = 0;               /* null terminate like fgets() */
    return(n);
}
/* end readline */

ssize_t isshe_readline(int fd, void *ptr, size_t maxlen)
{
    ssize_t n;

    if ( (n = readline(fd, ptr, maxlen)) < 0) {
        isshe_sys_error_exit("readline error");
    }

    return(n);
}

off_t isshe_lseek(int fd, off_t offset, int whence)
{
    off_t pos;

    if ( (pos = lseek(fd, offset, whence)) == (off_t) -1) {
        isshe_sys_error_exit("lseek error");
    }

    return(pos);
}

void isshe_ftruncate(int fd, off_t length)
{
    if (ftruncate(fd, length) == -1) {
        isshe_sys_error_exit("ftruncate error");
    }
}

