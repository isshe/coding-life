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