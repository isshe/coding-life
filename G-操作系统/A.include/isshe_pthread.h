#ifndef _ISSHE_THREAD_H_
#define _ISSHE_THREAD_H_

#include <pthread.h>

void isshe_pthread_create(pthread_t *tid, const pthread_attr_t *attr,
    void * (*func)(void *), void *arg);

void isshe_pthread_join(pthread_t tid, void **status);

#endif