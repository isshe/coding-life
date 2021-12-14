[TOC]

# 读写锁

## 1. 什么是读写锁
* 互斥锁把试图进入临界区的(多余)线程都阻塞住，无论是读还是写。（不区分读写）
    * 独占锁。
* 读写锁区分读和写，同时只能一个线程写(写时不能读)；允许同时多个线程读(读时没有写)。
    * 读为共享锁，写为独占锁。
    * 读比写更频繁的应用中，用读写锁更好。

## 2. 读写锁的分配规则
* 只要没有线程持有读写锁`用于写`，任意数目的线程可以持有该读写锁`用于读`。
* 仅当没有线程持有读写锁时(用于读或写)，才能分配读写锁`用于写`。

## 3. 相关函数
* 静态分配用`PTHREAD_RWLOCK_INITIALIZER`进行初始化。
```c
#include <pthread.h>

// 初始化、销毁
int pthread_rwlock_init(pthread_rwlock_t *lock, const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *lock);

// 加锁、解锁
int pthread_rwlock_rdlock(pthread_rwlock_t *lock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *lock);
int pthread_rwlock_wrlock(pthread_rwlock_t *lock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *lock);
int pthread_rwlock_unlock(pthread_rwlock_t *lock);

// 属性初始化、销毁
int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);

// 属性设置：设置进程间共享: PTHREAD_PROCESS_SHARED/PTHREAD_PROCESS_PRIVATE
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
```
