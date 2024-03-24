[TOC]

# 记录上锁
* Posix 记录上锁的粒度是`单个字节`。
* Posix 记录上锁是`劝告性上锁(advisory locking)`。【1】
    * 内核维护着已由各个进程上锁的所有文件的正确信息；
    * 但不能防止一个进程写已由另一个进程读锁定的某个文件；
    * 不能防止一个进程读已由另一个进程写锁定的某个文件；

## 1. 相关接口
```c
#include <fcntl.h>

int fcntl(int fd, int cmd, .../* struct flock *arg */);

struct flock {
    off_t       l_start;    /* starting offset */
    off_t       l_len;      /* len = 0 means until end of file */
    pid_t       l_pid;      /* lock owner */
    short       l_type;     /* lock type: read/write, etc. */
    short       l_whence;   /* type of l_start */
};
```
* 锁整个文件：l_whence = SEEK_SET, l_start = 0, l_len = 0;
* cmd 取值：
    * F_SETLK: 设置锁（加读锁、加写锁、解锁）
    * F_SETLKW: 和`F_SETLK`类似，但是当无法获得锁的时候，阻塞到能获得锁为止。
    * F_GETLK: 获取锁。可以用来判断锁是否存在。
        * 当 F_SETLK 错误时，可以调用 F_GETLK 获取导致该错误的`锁的信息`。

## 2. 注意
* 当不是用读打开时，加读锁将会出错；
* 当不是用写打开时，加写锁将会出错；
* 当进程关闭文件时，所有的锁也会被清除。
* 锁不能通过 fork() 由子进程继承。
* `记录上锁`不应该和标准 I/O 函数库一起使用，因为该函数库会执行内部缓冲。当某个文件需上锁时，为避免问题，`应对它使用read和write`。
* `struct flock`的成员顺序根据实现不同，因此最好不要定义时初始化！如下：
    * `static struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0}`
* 

## 3. 用途
* 确保某个程序 (如守护进程) 在任意时刻只有一个实例在运行。
    * 见示例[5_ex](./Examples/5_ex_one_daemon.c)

# A.疑问
* [1] macOS 10.14.4 使用[2_ex](./Examples/2_ex_lock_fcntl.c)实测，锁期间还能修改文件内容。
* [2] 如果某个资源已经被读锁定，并有一个写入锁在等待，那么是否允许有另一个读出锁？
    * 示例详见[3_ex](./Examples/3_ex_test_wlock_rlock.c)
    * 结果：
        * macOS 10.14.4 下，允许第二个读锁。
        * Ubuntu 18.04 下，允许第二个读锁。
    
* [3] 等待的写锁是否比读等待的锁优先级高？
    * 示例详见[4_ex](./Examples/4_ex_test_wlock_rlock_prio.c)
    * 结果：
        × Ubuntu 18.04：看着像是随机的，运行两次，两次优先级不同
