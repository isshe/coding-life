[TOC]

# 信号
* 一个信号就是一条小消息，它通知进程系统中发生了一个某种类型的事件。
* 每种信号类型都对应于某种系统事件。
* 信号的默认行为：

## 1.信号发送和信号接收
* `待处理信号`：一个发出而没有被接收的信号。
* 内核通过更新目的进程上下文的某个状态，发送一个信号给目的进程。
* 当目的进程被内核`强迫`以某种方式对信号的发送做出反应时，目的进程就接收了信号。
* Unix系统提供了大量向进程发送信号的机制，所有这些机制都是基于`进程组(process group)`的。
* 相关函数:
```c
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

// 发送sig指定信号到pid进程
int kill(pid_t pid, int sig);

// 向自己发送SIGALRM信号
unsigned int alarm(unsigned int secs);

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

## 2.信号处理问题
* 待处理信号被阻塞。
  * 例如：一个进程捕获一个信号SIGINT然后去处理程序处理信号，此时又来一个SIGINT信号，此信号会被阻塞。[疑问][1]
  * （第2个信号不是SIGINT信号，那是否也还是阻塞？）
* 待处理信号不会排队等待。
  * 任意类型至多只有一个待处理信号。[疑问][2]
  * （不同类型的信号就可以有多个吗？）
* 系统调用可以被中断。
  * 如read、wait、accept这样的`慢速系统调用`被信号打断，但是信号处理函数返回后不再继续被打断的系统调用，而是立即返回错误，errno为`EINTR`。

详见[Examples/ex_SIGCHLD]
**教训：不可以用信号来对其他进程中发生的时间计数**

## A.拓展
* 进程组。【见`1.进程`】
* Linux信号
![Linux信号](./Linux_signal.png)
* 回收子进程的方式：用SIGCHLD信号。子进程终止时，会发送SIGCHLD信号给其父进程。【详见[ex_SIGCHILD.c](./Examples/ex_SIGCHLD.c)】
* 可移植的signal函数。【详见[isshe_signal.h](../../A.lib/isshe_signal.h)和[isshe_signal.c](../../A.lib/isshe_signal.c)】

## B.疑问
* `2.信号处理问题`中：
[1]第2个信号不是SIGINT信号，那是否也还是阻塞？
[2]不同类型的信号就可以有多个吗？
