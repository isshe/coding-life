[TOC]

# FIFO
* FIFO 有时被称为命名管道。
* FIFO 是一种文件类型。
* 支持在无亲缘进程间使用。

## 1. FIFO 创建
* 创建 FIFO 类似于创建文件；FIFO 的路径名存在于文件系统中。

## 2. 创建 FIFO
* 相关函数
```c
#include <sys/stat.h>
int mkfifo(const char *path, mode_t mode);
// 在 fd 表示的目录相关位置，创建一个 FIFO。
// * path 为绝对路径，则忽略 fd
// * path 为相对路径，则 fd 参数是一个打开的目录的文件描述符。路径名和目录有关【？？？】
// * path 为相对路径，并且 fd 参数有特殊值 AT_FDCWD，则路径名以当前目录开始。
int mkfifoat(int fd, const char *path, mode_t mode);
```

## 3. 其他
* open 一个 FIFO 时，非阻塞标记 (O_NONBLOCK) 会产生的影响：
    * 一般情况下 (没有指定 O_NONBLOCK)，
        * 只读 open 要阻塞到其他进程为写打开此 FIFO 为止；
        * 只写 open 要阻塞到其他进程为读打开此 FIFO 为止；
    * 如果指定了 O_NONBLOCK，则：
        * 只读 open 时，立即返回。【返回啥？正常返回？】
        * 只写 open 时，如果没有其他进程为读打开此 FIFO，则此只写 open 返回 -1，errno 置为 ENXIO。
* 若 write 一个没有进程为读打开的 FIFO，则产生信号 SIGPIPE。（类似 pipe）
* 若 FIFO 的最后一个写进程关闭了该 FIFO，则将为改 FIFO 的读进程产生一个文件结束标记。（类似于 pipe）
* FIFO 的两种用途：
    * shell 命令使用 FIFO 将数据从一条管道传送到另一条时，无需创建中间临时文件；
    * 客户——服务器进程应用中，FIFO 用作汇聚点，在客户和服务器进程间传递数据；

## 4.管道和 FIFO 的限制
* OPEN_MAX: 一个进程在任意时刻打开的最大描述符数。（sysconf 函数获取）
    * 示例见[3_ex_pipeconf.c](./Examples/3_ex_pipeconf.c)
* PIPE_BUF: 可原子写管道/FIFO 的最大数据量。（pathconf/fpathconf 函数获取）
    * 示例见[3_ex_pipeconf.c](./Examples/3_ex_pipeconf.c)

## A. 疑问

## B. 参考
* 《unix 环境高级编程 第三版》
* 《UNIX 网络编程 卷 2 进程间通信 第 2 版》

