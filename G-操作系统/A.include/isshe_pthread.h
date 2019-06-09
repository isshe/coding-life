#ifndef _ISSHE_THREAD_H_
#define _ISSHE_THREAD_H_

#include <pthread.h>

int isshe_pthread_create(pthread_t *tid, const pthread_attr_t *attr, void * (*func)(void *), void *arg);
int isshe_pthread_join(pthread_t tid, void **status);
int isshe_pthread_cancel(pthread_t tid);
int isshe_pthread_detach(pthread_t tid);
void isshe_pthread_exit(void *retval);
pthread_t isshe_pthread_self(void);
void isshe_pthread_once(pthread_once_t *once_control, void (*init_function)());

int isshe_pthread_mutex_lock(pthread_mutex_t *mptr);
int isshe_pthread_mutex_unlock(pthread_mutex_t *mptr);

int isshe_pthread_cond_signal(pthread_cond_t *cptr);
int isshe_pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
int isshe_pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *tsptr);

pthread_t isshe_thread_id(pthread_t *ptr);

#endif