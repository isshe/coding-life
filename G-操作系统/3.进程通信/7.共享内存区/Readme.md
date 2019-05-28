[TOC]

# Posix共享内存区
* 共享内存区是可用IPC形式中最快的。
* 一旦内存区映射到共享它的进程的地址空间，这些进程间的数据传递就不需要经过内核了；但是读写内存区时，需要进行同步。
* 非共享缓冲区读文件传给另一个进程写：4次内核与内核的交互
![非共享缓冲区读文件传给另一个进程写](./not_share_memory.png)
* 共享缓冲区读文件传给另一个进程写：2次内核与进程的交互
![共享缓冲区读文件传给另一个进程写](./share_memory.png)

## 1. 相关函数

### 1.1 mmap
* mmap: 把一个文件或一个Posix共享内存区对象映射到调用进程的地址空间。
* 使用此函数有3个目的：
    * 使用普通文件以提供内存映射I/O；
    * 使用特殊文件以提供匿名内存映射；
    * 使用shm_open以提供无亲缘关系进程间的Posix共享内存区。
* 映射文件示意图：
![映射文件示意图](./map_file.png)

```c
#include <sys/mman.h>

// addr: 指定fd映射到的进程内空间的起始地址；
// len: 映射的长度——字节数；
// offset: 从被映射文件开头起offset字节开始映射；
// prot: 属性/模式(PROT_READ|PROT_WRITE|PROT_EXEC|PROT_NONE)
//  * PROT_NONE   Pages may not be accessed.
//  * PROT_READ   Pages may be read.
//  * PROT_WRITE  Pages may be written.
//  * PROT_EXEC   Pages may be executed.
// flag: 很多歌，具体见man
//  * MAP_PRIVATE: 变动私有，不改变低层支撑对象；
//  * MAP_SHARED: 变动共享，其他进程可见，改变低层支撑对象；
//  * MAP_FIXED: 准确地解释addr参数；【？？？】
// 返回：成功——映射起始地址；失败——MAP_FAILED，设置errno
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
```

### 1.2 munmap
* munmap: 从某个进程的地址空间删除一个映射关系。
```c
#include <sys/mman.h>

int munmap(void *addr, size_t len);
```

### 1.3 msync
* msync: 同步内存与硬盘上的内容。
```c
#include <sys/mman.h>

// flags: 
//  * MS_ASYNC: 执行异步写（写操作入内核队列，就返回）
//  * MS_SYNC: 执行同步写（写操作完成后才返回）
//  （以上两个指定一个，但不能都指定）
//  * MS_INVALIDATE: 使高速缓存的数据失效
int msync(void *addr, size_t len, int flags);
```

## 2. 文件内存映射
内存映射一个普通文件时，内存中映射区的大小(mmap的第2个参数)通常等于改文件的大小。
详见[示例](./Examples/2_ex_shm_increase2.c)

## 3. 匿名内存映射
使用非匿名内存映射时，需要在文件系统中创建一个文件，进行open并write一些数据进行初始化。
如果目的是提供一个父子进程共享的内存映射，匿名内存映射则能简化上述流程。
创建匿名映射的方法：
* mmap的flag参数指定`MAP_SHARED|MAP_ANON`, fd = -1。
* 这样的内存区会被初始化为0；
详见[示例](./Examples/3_ex_increase_map_anon.c)

## A. 注意
* 从移植性考虑，MAP_FIXED不应该指定。
* 可移植的代码，应该把addr指定为NULL，并且不指定MAP_FIXED。
* mmap成功返回后，fd可关闭。
* 不是所有文件都能mmap

## B.问题
* 为什么使用mmap？
    * 不用调用read/write/lseek，简化了代码。




