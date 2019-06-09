#include <errno.h>

#include "isshe_pthread.h"
#include "isshe_error.h"
#include "isshe_common.h"

int isshe_pthread_create(pthread_t *tid, const pthread_attr_t *attr,
                            void * (*func)(void *), void *arg)
{
    int rc;

    if ( (rc = pthread_create(tid, attr, func, arg)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_create error");
    }

    return rc;
}

int isshe_pthread_join(pthread_t tid, void **status)
{
    int rc;

    if ( (rc = pthread_join(tid, status)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_join error");
    }

    return rc;
}

int isshe_pthread_cancel(pthread_t tid)
{
    int rc;

    if ((rc = pthread_cancel(tid)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_cancel error");
    }

    return rc;
}

int isshe_pthread_detach(pthread_t tid)
{
    int rc;

    if ((rc = pthread_detach(tid)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_detach error");
    }

    return rc;
}

void isshe_pthread_exit(void *retval)
{
    pthread_exit(retval);
}

pthread_t isshe_pthread_self(void)
{
    return pthread_self();
}

void isshe_pthread_once(pthread_once_t *once_control, void (*init_function)())
{
    pthread_once(once_control, init_function);
}

int isshe_pthread_mutex_lock(pthread_mutex_t *mptr)
{
    int rc;

    if ( (rc = pthread_mutex_lock(mptr)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_mutex_lock error");
    }

    return rc;
}


int isshe_pthread_mutex_unlock(pthread_mutex_t *mptr)
{
    int rc;

    if ( (rc = pthread_mutex_unlock(mptr)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_mutex_unlock error");
    }

    return rc;
}

int isshe_pthread_cond_signal(pthread_cond_t *cptr)
{
    int rc;

    if ( (rc = pthread_cond_signal(cptr)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_cond_signal error");
    }

    return rc;
}

int isshe_pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
    int rc;

    if ( (rc = pthread_cond_wait(cptr, mptr)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_cond_wait error");
    }

    return rc;
}

int isshe_pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr,
                        const struct timespec *tsptr)
{
    int rc;

    if ( (rc = pthread_cond_timedwait(cptr, mptr, tsptr)) != ISSHE_SUCCESS) {
        isshe_posix_error_exit(rc, "pthread_cond_timedwait error");
    }

    return rc;
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