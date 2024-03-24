[TOC]

# System V 共享内存区


## 1. 概述
* System V 共享内存区在概念上类似于 Posix 共享内存区；
    * Posix 共享内存区：先调用 shm_open，后调用 mmap;
    * System V 共享内存区：先调用 shmget, 后调用 shmat;

## 2. shmget 函数
```c
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
```
* 作用：创建一个新的共享内存区或者访问一个已存在的共享内存区。
* 参数：
    * key: ftok 的返回值或者 IPC_PRIVATE。
        * 如果 key=IPC_PRIVATE 并且 shmflg 指定 IPC_CREAT 标记，则创建共享内存区。
        * 如果没有 key 对应的共享内存标识符并且指定 IPC_CREAT，则创建共享内存区。
        * 创建内存区会初始化内存区为 0。（size 字节）
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
    * shmflg：标记
* 返回：共享内存区标识符 (整数)

## 3. shmat 函数
```c
void *shmat(int shmid, const void *shmaddr, int shmflg);
```
* 作用：把 shmget 创建/打开的共享内存区连接到调用进程的地址空间；
* 参数：
    * shmid: shmget 返回的标识符，共享内存区的 ID；
    * shmaddr：如果是 NULL，则系统进行选址（推荐）；
        * 不为 NULL：
            * shmflg 指定了`SHM_RND`，连接到 shmaddr 指定的地址；
            * shmflg 没有指定`SHM_RND`，连接到 shmaddr 指定的地址向下舍入的一个 SHMLBA 常值；
            * LBA 表示：低端边界地址 (lower bounder address)【？？？】
    * shmflg: 标记/权限；
* 返回：成功，映射区的起始地址；失败，-1。

## 4. shmdt 函数
```c
int shmdt(const void *shmaddr)
```
* 作用：断开与共享内存区的连接。
* 参数：
    * shmaddr：共享内存区地址
* 返回：成功 0，失败 -1

## 5. shmctl 函数
```c
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```
* 作用：对一个共享内存区进行控制/操作。
* 参数：
    * shmid: 共享内存区 ID；
    * cmd: 控制命令，IPC_RMID/IPC_SET/IPC_STAT;
    * buff: 传参/传结果的结构
* 返回：成功 0，失败 -1

## 6. System V 共享内存区的限制
和 System V 消息队列、System V 信号量一样，System V 共享内存区也存在特定的系统限制。
* 详见[示例 5_ex](./Examples/5_ex_svshm_limites.c)

## A. 问题
* shmget 是否要求文件存在？
    * 要求文件存在。
* 是否支持亲缘进程间使用？
    * 支持。
    * 具体哪里进行拆分，没有进行测试。
* 是否支持无亲缘进程间使用？
    * 支持。
* 读/写超过共享内存区范围会怎样？
    * 大小分页后，最后不足 1 页：
        * 访问不超过此页，不会报错；
        * 超过，则段错误 (segmentation fault)。
    * 【和[7.B](../7.共享内存区/Readme.md)类似】
* Posix 共享内存区和 SystemV 共享内存区有什么差别？
    * Posix 共享内存区对象的大小可以再任意时刻通过调用 ftruncate`改变`；
    * SystemV 共享内存区对象的大小是调用 shmget 创建时`固定`的。

## B. 注意
* shmget 创建或打开共享内存区时，并没有给调用进程提供访问该内存区的手段。（因此要调用 shmat）




