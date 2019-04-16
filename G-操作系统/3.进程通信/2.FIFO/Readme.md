[TOC]

# FIFO
* FIFO有时被称为命名管道。
* FIFO是一种文件类型。

## 1. FIFO创建
* 创建FIFO类似于创建文件；FIFO的路径名存在于文件系统中。
* 相关函数
```c
#include <sys/stat.h>
int mkfifo(const char *path, mode_t mode);
// 在fd表示的目录相关位置，创建一个FIFO。
// * path为绝对路径，则忽略fd
// * path为相对路径，则fd参数是一个打开的目录的文件描述符。路径名和目录有关【？？？】
// * path为相对路径，并且fd参数有特殊值AT_FDCWD，则路径名以当前目录开始。
int mkfifoat(int fd, const char *path, mode_t mode);
```

## 2. 其他
* open一个FIFO时，非阻塞标记(O_NONBLOCK)会产生的影响：
    * 一般情况下(没有指定O_NONBLOCK)，
        * 只读open要阻塞到其他进程为写打开此FIFO为止；
        * 只写open要阻塞到其他进程为读打开此FIFO为止；
    * 如果制定了O_NONBLOCK，则：
        * 只读open时，立即返回。【返回啥？正常返回？】
        * 只写open时，如果没有其他进程为读打开此FIFO，则此只写open返回-1，errno置为ENXIO。
* 若write一个没有进程为读打开的FIFO，则产生信号SIGPIPE。（类似pipe）
* 若FIFO的最后一个写进程关闭了该FIFO，则将为改FIFO的读进程产生一个文件结束标记。（类似于pipe）
* FIFO的两种用途：
    * shell命令使用FIFO将数据从一条管道传送到另一条时，无需创建中间临时文件；
    * 客户——服务器进程应用中，FIFO用作汇聚点，在客户和服务器进程间传递数据；

## A. 疑问
* 学习完还是一脸懵逼。没有示例。

## B. 参考
* 《unix环境高级编程 第三版》

