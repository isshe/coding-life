#include <errno.h>

#include "isshe_pthread.h"
#include "isshe_error.h"
#include "isshe_common.h"

void isshe_pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                            void * (*func)(void *), void *arg)
{
    int n;

    if ( (n = pthread_create(tid, attr, func, arg)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_create error");
}

void isshe_pthread_join(pthread_t tid, void **status)
{
    int n;

    if ( (n = pthread_join(tid, status)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_join error");
}


void isshe_pthread_mutex_lock(pthread_mutex_t *mptr)
{
    int n;

    if ( (n = pthread_mutex_lock(mptr)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_mutex_lock error");
}


void isshe_pthread_mutex_unlock(pthread_mutex_t *mptr)
{
    int n;

    if ( (n = pthread_mutex_unlock(mptr)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_mutex_unlock error");
}

void isshe_pthread_cond_signal(pthread_cond_t *cptr)
{
    int n;

    if ( (n = pthread_cond_signal(cptr)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_cond_signal error");
}

void isshe_pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
    int n;

    if ( (n = pthread_cond_wait(cptr, mptr)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_cond_wait error");
}

void isshe_pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr,
                        const struct timespec *tsptr)
{
    int n;

    if ( (n = pthread_cond_timedwait(cptr, mptr, tsptr)) == ISSHE_SUCCESS) {
        return;
    }

    errno = n;
    isshe_sys_error_exit("pthread_cond_timedwait error");
}

pthread_t isshe_thread_id(pthread_t *ptr)
{
#if defined(sun)
	return((ptr == NULL) ? pthread_self() : *ptr);	/* Solaris */

#elif defined(__osf__) && defined(__alpha)
	pthread_t	tid;

	tid = (ptr == NULL) ? pthread_self() : *ptr;	/* Digital Unix */
	return(pthread_getsequence_np(tid));
#else

	/* 4everything else */
	return((ptr == NULL) ? pthread_self() : *ptr);
#endif
}