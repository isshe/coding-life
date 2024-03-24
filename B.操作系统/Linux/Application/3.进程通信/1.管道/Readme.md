
[TOC]

# 管道

* 管道的局限性
    * 半双工：数据只能在一个方向上流动；
    * 只能在具有共同祖先的两个进程间使用。

## 1. 管道创建
* 相关函数
```c
#include <unistd.h>

int pipe(int fd[2]);
```
* `fd[0]`为读而打开；
* `fd[1]`为写而打开；
* `fd[1]`的输出是`fd[0]`的输入；【？！】
* `PIPE_BUF`规定内核的管道缓冲区的大小。
    * `pathconf`及`fpathconf`可以确定 PIPE_BUF 的值。

* 当管道的一端被关闭后，适用的规则：
    * 当读 (read) 一个写端被关闭的管道，在所有数据被读取后，read 返回 0；
    * 当写 (write) 一个读端被关闭的管道，则产生信号`SIGPIPE`。
        * 忽略/捕捉信号并从信号处理程序返回后，write 返回 -1，errno=EPIPE。

# A. 疑问
* `fd[1]`的输出是`fd[0]`的输入？
    * `fd[1]写端`往管道写数据，即`fd[1]`的输出。
    * `fd[0]读端`从管道读数据，即`fd[0]`的输入。
    * 详见示例：[2_ex_pipe_copy_file_toless_or_more.c](./Examples/2_ex_pipe_copy_file_to_less_or_more.c)
* 多个进程使用相同的`fd[2]`的时候，管道的读写是怎么样的？
    * 管道是多个进程共用的，谁都可以写，也都可以读。因此写入/读取的信息可能混杂在一起。
    * 详见示例：[3_ex_read_and_write.c](./Examples/3_ex_read_and_write.c)