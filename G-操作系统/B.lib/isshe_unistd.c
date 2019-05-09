#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "isshe_unistd.h"
#include "isshe_error.h"

long isshe_pathconf(const char *pathname, int name)
{
    long    val;

    errno = 0;  /* in case pathconf() does not change this */
    if ( (val = pathconf(pathname, name)) == -1) {
        if (errno != 0) {
            isshe_sys_error_exit("pathconf error");
        }
        else {
            isshe_sys_error_exit("pathconf: %d not defined", name);
        }
    }
    return(val);
}

long isshe_sysconf(int name)
{
    long    val;

    errno = 0;  /* in case sysconf() does not change this */
    if ( (val = sysconf(name)) == -1) {
        if (errno != 0) {
            isshe_sys_error_exit("sysconf error");
        }
        else {
            isshe_sys_error_exit("sysconf: %d not defined", name);
        }
    }
    return(val);
}

int isshe_fcntl(int fd, int cmd, void *arg)
{
    int	n;

    if ( (n = fcntl(fd, cmd, arg)) == -1) {
        isshe_sys_error_exit("fcntl error");
    }

    return(n);
}

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock	lock;

    lock.l_type = type;		/* F_RDLCK, F_WRLCK, F_UNLCK */
    lock.l_start = offset;	/* byte offset, relative to l_whence */
    lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
    lock.l_len = len;		/* #bytes (0 means to EOF) */

    return( fcntl(fd, cmd, &lock) );	/* -1 upon error */
}

void isshe_lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    if (lock_reg(fd, cmd, type, offset, whence, len) == -1) {
        isshe_sys_error_exit("lock_reg error");
    }
}

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;		/* F_RDLCK or F_WRLCK */
    lock.l_start = offset;	/* byte offset, relative to l_whence */
    lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
    lock.l_len = len;		/* #bytes (0 means to EOF) */

    if (fcntl(fd, F_GETLK, &lock) == -1)
        return(-1);			/* unexpected error */

    if (lock.l_type == F_UNLCK)
        return(0);			/* false, region not locked by another proc */
    return(lock.l_pid);		/* true, return positive PID of lock owner */
}

pid_t isshe_lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    pid_t pid;

    if ( (pid = lock_test(fd, type, offset, whence, len)) == -1) {
        isshe_sys_error_exit("lock_test error");
    }

    return(pid);
}