[TOC]

# System V 共享内存区


## 1. 概述
* System V 共享内存区在概念上类似于Posix共享内存区；
    * Posix共享内存区：先调用shm_open，后调用mmap;
    * System V 共享内存区：先调用shmget, 后调用shmat;

## 2. shmget函数
```c
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
```
* 作用：创建一个新的共享内存区或者访问一个已存在的共享内存区。
* 参数：
    * key: ftok的返回值或者IPC_PRIVATE。
        * 如果key=IPC_PRIVATE并且shmflg指定IPC_CREAT标记，则创建共享内存区。
        * 如果没有key对应的共享内存标识符并且指定IPC_CREAT，则创建共享内存区。
        * 创建内存区会初始化内存区为0。（size字节）
        * 创建共享内存区会分配`shmid_ds结构`
        ```c
             struct shmid_ds {
                 struct ipc_perm  shm_perm;     /* operation permissions */
                 int              shm_segsz;    /* size of segment in bytes */
                 pid_t            shm_lpid;     /* pid of last shm op */
                 pid_t            shm_cpid;     /* pid of creator */
                 short            shm_nattch;   /* # of current attaches */
                 time_t           shm_atime;    /* last shmat() time*/
                 time_t           shm_dtime;    /* last shmdt() time */
                 time_t           shm_ctime;    /* last change by shmctl() */
                 void            *shm_internal; /* sysv stupidity */
             };
        ```
    * size: 大小
    * shmflg： 标记
* 返回：共享内存区标识符(整数)


## A. 问题
* 是否支持亲缘进程间使用？
* 是否支持无亲缘进程间使用？

## B. 注意
* shmget创建或打开共享内存区时，并没有给调用进程提供访问该内存区的手段。（因此要调用shmat）




