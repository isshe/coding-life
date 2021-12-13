#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "isshe_unistd.h"
#include "isshe_error.h"
#include "isshe_common.h"

long isshe_pathconf(const char *pathname, int name)
{
    long    val;

    errno = 0;  /* in case pathconf() does not change this */
    if ( (val = pathconf(pathname, name)) == ISSHE_FAILURE) {
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
    if ( (val = sysconf(name)) == ISSHE_FAILURE) {
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

    if ( (n = fcntl(fd, cmd, arg)) == ISSHE_FAILURE) {
        isshe_sys_error_exit("fcntl error");
    }

    return(n);
}

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;		/* F_RDLCK or F_WRLCK */
    lock.l_start = offset;	/* byte offset, relative to l_whence */
    lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
    lock.l_len = len;		/* #bytes (0 means to EOF) */

    if (fcntl(fd, F_GETLK, &lock) == ISSHE_FAILURE)
        return(ISSHE_FAILURE);			/* unexpected error */

    if (lock.l_type == F_UNLCK)
        return(0);			/* false, region not locked by another proc */
    return(lock.l_pid);		/* true, return positive PID of lock owner */
}

pid_t isshe_lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    pid_t pid;

    if ( (pid = lock_test(fd, type, offset, whence, len)) == ISSHE_FAILURE) {
        isshe_sys_error_exit("lock_test error");
    }

    return(pid);
}

int isshe_getopt(int argc, char *const *argv, const char *str)
{
    int opt;

    if ( ( opt = getopt(argc, argv, str)) == '?') {
        exit(1);        /* getopt() has already written to stderr */
    }

    return(opt);
}

void *isshe_calloc(size_t n, size_t size)
{
    void *ptr;

    if ( (ptr = calloc(n, size)) == NULL) {
        isshe_sys_error_exit("calloc error");
    }

    return(ptr);
}

void *isshe_malloc(size_t size)
{
    void    *ptr;

    if ( (ptr = malloc(size)) == NULL) {
        isshe_sys_error_exit("malloc error");
    }

    return(ptr);
}

void isshe_free(void *ptr)
{
    if (ptr) {
        free(ptr);
    } else {
        isshe_error("want to free(NULL)");
    }
}

void *isshe_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void    *ptr;

    if ( (ptr = mmap(addr, len, prot, flags, fd, offset)) == MAP_FAILED ) {
        isshe_sys_error_exit("mmap error");
    }
    return(ptr);
}

void isshe_munmap(void *addr, size_t len)
{
    if (munmap(addr, len) == -1) {
        isshe_sys_error_exit("munmap error");
    }
}


int sleep_us(unsigned int nusecs)
{
    struct timeval  tval;

    if (nusecs == 0) {
        return(0);
    }

    for ( ; ; ) {
        tval.tv_sec = nusecs / 1000000;
        tval.tv_usec = nusecs % 1000000;
        if (select(0, NULL, NULL, NULL, &tval) == 0) {
            return(0);      /* all OK */
        }

        /*
         * Note that on an interrupted system call there's not
         * much we can do, since the timeval{} isn't updated with the time
         * remaining.  We could obtain the clock time before the call, and
         * then obtain the clock time here, subtracting them to determine
         * how long select() blocked before it was interrupted, but that
         * seems like too much work :-)
         */
        if (errno != EINTR)
            return(-1);
        /* else go around again */
    }
}

void isshe_sleep_us(unsigned int nusecs)
{
    if (sleep_us(nusecs) == -1) {
        isshe_sys_error_exit("sleep_us error");
    }
}

int isshe_select(int nfds, fd_set *readfds,
    fd_set *writefds,  fd_set *exceptfds, struct timeval *timeout)
{
    int n;

again_select:
    if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0) {
        if (errno == EINTR) {
            goto again_select;
        } else {
            isshe_sys_error_exit("select error");
        }
    } else if (n == 0 && timeout == NULL) {
        isshe_error_exit("select returned 0 with no timeout");
    }

    return(n);  /* can return 0 on timeout */
}

int isshe_poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
    int n;

    if ( (n = poll(fdarray, nfds, timeout)) < 0) {
        isshe_error_exit("poll error");
    }

    return(n);
 }

#ifdef __linux__
int isshe_epoll_create(int flags)
{
    int rc;

    if ( (rc = epoll_create1(flags)) < 0 ) {
        isshe_sys_error_exit("epoll_create1 error");
    }

    return(rc);
}

int isshe_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
    int rc;

    if ( (rc = epoll_ctl(epfd, op, fd, event)) < 0 ) {
        isshe_sys_error_exit("epoll_ctl error");
    }

    return(rc);
}

int isshe_epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout,
               const sigset_t *sigmask)
{
    int rc;

    if (sigmask) {
        if ( (rc = epoll_pwait(epfd, events, maxevents, timeout, sigmask)) < 0 ) {
            isshe_sys_error_exit("epoll_pwait error");
        }
    } else {
        if ( (rc = epoll_wait(epfd, events, maxevents, timeout)) < 0 ) {
            isshe_sys_error_exit("epoll_wait error");
        }
    }

    return(rc);
}
#endif

#if defined(__bsdi__) || defined(__APPLE__)
int isshe_kqueue(void)
{
    int rc;

    if ((rc = kqueue()) == ISSHE_FAILURE){
        isshe_sys_error_exit("kqueue error");
    }

    return rc;
}

int isshe_kevent(int kq, const struct kevent *changelist, int nchanges,
        struct kevent *eventlist, int nevents,
        const struct timespec *timeout)
{
    int rc;

    if ((rc = kevent(kq, changelist, nchanges, eventlist, nevents, timeout)) == ISSHE_FAILURE){
        isshe_sys_error_exit("kevent error");
    }

    return rc;
}

int isshe_kevent64(int kq, const struct kevent64_s *changelist, int nchanges,
        struct kevent64_s *eventlist, int nevents, unsigned int flags,
        const struct timespec *timeout)
{
    int rc;

    if ((rc = kevent64(kq, changelist, nchanges, eventlist, nevents, flags, timeout)) == ISSHE_FAILURE){
        isshe_sys_error_exit("kevent64 error");
    }

    return rc;
}
#endif