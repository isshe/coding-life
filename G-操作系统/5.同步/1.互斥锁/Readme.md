[TOC]

# 互斥锁
* 互斥锁（及条件变量）是同步的基本组成部分。
* 互斥锁（及条件变量）出自`Posix.1`线程标准；
    * 可以用来同步一个进程内的各个线程；
    * 也可以用在进程间同步：互斥锁放在多个进程的共享内存区中。
* 互斥锁：相互排斥，是最基本的同步形式。
* 作用：保护临界区（critical region）。
    * 保护的是数据：临界区中被操纵的数据；进程/线程共享的数据。
* 互斥锁是`协作性`锁，也就是锁无法防止一些进程/线程不先获取锁就访问数据。（需要靠自觉）

## 1. 上锁/解锁
```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mptr);
int pthread_mutex_trylock(pthread_mutex_t *mptr);
int pthread_mutex_unlock(pthread_mutex_t *mptr);

// 均返回：成功：0，失败：正的Exxx值
```

## A. 拓展
* 生产者-消费者问题，也称为`有界缓冲区`问题。
    * 当生产者-消费者使用管道、消息队列(SystemV/Posix)进行通信时，同步是`隐式的(implicit)`，由内核执行同步。
    * 当使用内存共享时，生产者/消费者必须执行某种`显式的(explicit)`的同步。

    