#ifndef _ISSHE_THREAD_H_
#define _ISSHE_THREAD_H_

#include <pthread.h>

void isshe_pthread_create(pthread_t *tid, const pthread_attr_t *attr, void * (*func)(void *), void *arg);
void isshe_pthread_join(pthread_t tid, void **status);

void isshe_pthread_mutex_lock(pthread_mutex_t *mptr);
void isshe_pthread_mutex_unlock(pthread_mutex_t *mptr);

void isshe_pthread_cond_signal(pthread_cond_t *cptr);
void isshe_pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
void isshe_pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *tsptr);

#endif