#include <errno.h>

#include "isshe_pthread.h"
#include "isshe_error.h"

void isshe_pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                            void * (*func)(void *), void *arg)
{
    int n;

    if ( (n = pthread_create(tid, attr, func, arg)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_create error");
}

void isshe_pthread_join(pthread_t tid, void **status)
{
    int n;

    if ( (n = pthread_join(tid, status)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_join error");
}


void isshe_pthread_mutex_lock(pthread_mutex_t *mptr)
{
    int n;

    if ( (n = pthread_mutex_lock(mptr)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_mutex_lock error");
}


void isshe_pthread_mutex_unlock(pthread_mutex_t *mptr)
{
    int n;

    if ( (n = pthread_mutex_unlock(mptr)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_mutex_unlock error");
}

void isshe_pthread_cond_signal(pthread_cond_t *cptr)
{
    int n;

    if ( (n = pthread_cond_signal(cptr)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_cond_signal error");
}

void isshe_pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
    int n;

    if ( (n = pthread_cond_wait(cptr, mptr)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_cond_wait error");
}

void isshe_pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr,
                        const struct timespec *tsptr)
{
    int n;

    if ( (n = pthread_cond_timedwait(cptr, mptr, tsptr)) == 0) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_cond_timedwait error");
}