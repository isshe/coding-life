[TOC]

# Posix 共享内存区
Posix.1 提供了两种无亲缘关系进程间共享内存区的方法：（都需要调用 mmap）
* 内存映射文件 (memory-mapped file)：由 open 函数打开，由 mmap 函数把得到的描述符映射到当前进程地址空间中的一个文件。
* 共享内存区对象 (share-memory object)：由 shm_open 打开一个 Posix.1 IPC 名字，所返回的描述符由 mmap 函数映射到当前进程的地址空间。


## 1. 相关函数
```c
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// 只是打开一个内存区对象，返回 fd
// oflag: 必须函数 O_RDONLY、O_RDWR 中的一个。
// mode: 
//  * 指定权限为，在指定了 O_CREAT 的前提下使用。
//  * 与 mq_open 和 sem_open 不同，shm_open 的 mode 参数必须指定。
int shm_open(const char *name, int oflag, ...);
// 删除一个共享内存区对象的名字。（删除一个名字不会影响低层支撑对象的现有引用。）
int shm_unlink(const char *name);

// 裁剪普通文件或者共享内存区对象
int ftruncate(int fd, off_t length);

//获取打开的共享内存对象的信息
int fstat(int fildes, struct stat *buf);
```

## 2. 示例
* 共享计数器持续 +1
    * [示例 6_1](./Examples/6_1_ex_pxshm_server1.c)
    * [示例 6_2](./Examples/6_2_ex_pxshm_client1.c)
* 向服务器发送消息
    * ![](./one_server_multi_client.png)
    * 

## A.注意
* 在 macOS 10.14.5 中，shm_open 并不会创建对应的同名文件。
    * 详见[示例 1](./Examples/1_ex_shm_create.c)

## B.问题
* 为什么 shm_open 不直接返回共享内存的地址？
    * 因为 Posix.1 的共享内存发明之前，mmap 已经存在。
    * mmap 使用的是已打开的描述符。
* 是否支持亲缘进程间使用？先映射，再 fork，是否还能使用？
    * 支持。父进程中 open/shm_open；fork 后，分别 mmap。
    * 详见[示例 5_ex](./Examples/5_ex_pxshm_test3.c)
* 是否支持无亲缘进程间使用？
    * 支持。


