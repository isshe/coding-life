# System V 信号量


## 1. 概述
* 二值信号量(binary semaphore): 其值为0或1的信号量；
* 计数信号量(counting semaphore): 其值为0~某个限制值之间的信号量；
> * 以上两种信号量，等待(waiting)操作都等待信号量的值变为大于0；
* 计数信号量集(set of counting semaphores): 一个或多个信号量(构成一个集合), 其中的每个都是计数信号量。
    * System V信号量通过此概念给信号量增加了一级复杂度。
> 当讨论System V信号量时，都是指`计数信号量集`；当讨论Posix信号量时，都是指`计数信号量`。


## 2.信号量集相关数据结构
```
struct semid_ds {
    struct ipc_perm     sem_perm;   // 操作权限
    struct sem          *sem_base;  // 指向信号量集数组的指针
    ushort              sem_nsems;  // 信号量集中信号量的数量
    time_t              sem_otime;  // 最后semop()的时间
    time_t              sem_ctime;  // 最后创建或IPC_SET的时间
}

struct sem {
    ushort_t    semval;     // 信号量值
    short       sempid;     // 最后成功semop()/SETVAL, SETALL的PID
    ushort_t    semncnt;    // awaiting semval > current vale的数量
    ushort_t    semzcnt;    // awaiting semval = 0 的数量
}
``` 