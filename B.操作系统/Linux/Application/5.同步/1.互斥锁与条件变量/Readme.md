[TOC]

# 互斥锁与条件变量
* 互斥锁及条件变量是同步的基本组成部分。
* 互斥锁及条件变量出自`Posix.1`线程标准；
    * 可以用来同步一个进程内的各个线程；
    * 也可以用在进程间同步：互斥锁放在多个进程的共享内存区中。

## 1.互斥锁
* 互斥锁：相互排斥，是最基本的同步形式。
* 作用：保护临界区（critical region）。
    * 保护的是数据：临界区中被操纵的数据；进程/线程共享的数据。
* 互斥锁是`协作性`锁，也就是锁无法防止一些进程/线程不先获取锁就访问数据。（需要靠自觉）

### 1.1 相关函数
* 静态分配使用`PTHREAD_MUTEX_INITIALIZER`进行初始化
```c
#include <pthread.h>

// 互斥锁初始化、销毁
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

// 上锁、解锁
int pthread_mutex_lock(pthread_mutex_t *mptr);
int pthread_mutex_trylock(pthread_mutex_t *mptr);
int pthread_mutex_unlock(pthread_mutex_t *mptr);

// 属性初始化、销毁
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)

// 属性设置：进程间共享等（MACOS 和 LINUX 相关函数有所不同）

// 均返回：成功：0，失败：正的 Exxx 值
```

## 2. 条件变量
* 互斥锁用于上锁，条件变量用于等待。
* 每个条件变量总是与一个互斥锁相关联。

### 2.1 相关函数
* 静态分配使用`PTHREAD_COND_INITIALIZER`进行初始化
```c
#include <pthread.h>

// 条件变量初始化、销毁
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);

// 线程睡眠，进行等待
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
// 唤醒等待线程——等待在相应条件变量上的线程
int pthread_cond_signal(pthread_cond_t *cond);

// 唤醒等待线程——等待在相应条件变量上的多个线程
int pthread_cond_broadcast(pthread_cond_t *cond);

// 时间是绝对时间：返回时刻的系统时间。不是时间差。
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

struct timespec {
    time_t  tv_sec;     // seconds
    long    tv_nsec;    // nanoseconds
}

// 属性初始化、销毁
int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);

// 属性设置：进程间共享等（MACOS 和 LINUX 相关函数有所不同：MACOX 没有条件变量属性设置函数）
```

## 2. 注意
* 条件的检测是在互斥锁的保护下进行的；
    * 条件变量进行等待前，会释放持有的互斥锁。
    * 【详见[2_ex](./Examples/2_ex_prod_cons_cond.c)】

## A. 拓展
* 生产者 - 消费者问题，也称为`有界缓冲区`问题。
    * 当生产者 - 消费者使用管道、消息队列 (SystemV/Posix) 进行通信时，同步是`隐式的(implicit)`，由内核执行同步。
    * 当使用内存共享时，生产者/消费者必须执行某种`显式的(explicit)`的同步。

    