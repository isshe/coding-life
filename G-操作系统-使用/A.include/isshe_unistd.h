#ifndef _ISSHE_UNISTD_H_
#define _ISSHE_UNISTD_H_

#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <poll.h>


#ifdef __linux__
#include <sys/epoll.h>
#endif

#if defined(__bsdi__) || defined(__APPLE__)
#include <sys/event.h>
#endif


#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#endif

long isshe_pathconf(const char *pathname, int name);
long isshe_sysconf(int name);
int isshe_fcntl(int fd, int cmd, void *arg);
int isshe_getopt(int argc, char *const *argv, const char *str);
void *isshe_calloc(size_t n, size_t size);
void *isshe_malloc(size_t size);
void isshe_free(void *ptr);

void *isshe_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void isshe_munmap(void *addr, size_t len);
void isshe_sleep_us(unsigned int nusecs);

int isshe_select(int nfds, fd_set *readfds,
    fd_set *writefds,  fd_set *exceptfds, struct timeval *timeout);

int isshe_poll(struct pollfd *fdarray, unsigned long nfds, int timeout);

#ifdef __linux__
int isshe_epoll_create(int flags);
int isshe_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int isshe_epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout, const sigset_t *sigmask);
#endif

#if defined(__bsdi__) || defined(__APPLE__)
int isshe_kqueue(void);
int isshe_kevent(int kq, const struct kevent *changelist, int nchanges,
        struct kevent *eventlist, int nevents,
        const struct timespec *timeout);

int isshe_kevent64(int kq, const struct kevent64_s *changelist, int nchanges,
        struct kevent64_s *eventlist, int nevents, unsigned int flags,
        const struct timespec *timeout);
#endif

#endif