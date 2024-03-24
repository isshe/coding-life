[TOC]

# epoll
I/O 事件通知设施 (I/O event notification facility)。

## 1. epoll_create
```c
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_create1(int flags);
```
* 作用：打开一个 epoll 文件描述符。
* 参数：
    * size：Linux 2.6.8 后不再使用，但是必须大于 0。
    * flags：
        * 0：和 epoll_create 行为一样。
        * O_CLOEXEC: 进程退出时关闭文件描述符。
* 返回：
    * 失败：-1，设置 errno；否则：
    * 返回一个指向一个新 epoll 实例的文件描述符；当不再使用此描述符时，应使用 close 关闭。
    * 当所有指向此 epoll 实例的描述符都被关闭时，内核会释放相关资源。

## 2. epoll_ctl
```c
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};


int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);  
```
* 作用：epoll 文件描述符的控制接口。
* 参数：
    * epfd: epoll fd，epoll 文件描述符；
    * op:
        * EPOLL_CTL_ADD：注册 fd 到 epfd；
        * EPOLL_CTL_MOD：修改与 fd 的关联 evnet 事件。
        * EPOLL_CTL_DEL：从 epfd 中删除 fd；
    * fd: 要操作 (op) 的 fd；
    * event: 与 fd 关联的事件。EPOLL_CTL_DEL 时这个参数被忽略，也可以直接为 NULL；
* 返回：
    * 正常：0；
    * 异常：-1，设置 errno；
* 数据结构说明；
    * `struct epoll_event`的`events`的是位掩码的形式，取值是：
        * EPOLLIN: 相关文件可读。(read)
        * EPOLLOUT: 相关文件可写。(write)
        * EPOLLRDHUP: 流套接字对端关闭连接，或关闭写入一半的连接。(在使用边沿触发时，此标志对于编写简单代码以检测对端关闭特别有用。)
        * EPOLLPRI: 有紧急数据可读。
        * EPOLLERR: 相关文件描述符发生错误。（默认设置，不用手动设置）
        * EPOLLHUP: 相关文件描述符被挂起。（默认设置，不用手动设置）
        * EPOLLET: 相关文件描述符设置为`边缘触发`。（默认是水平触发）
        * EPOLLONESHOT: 对相关描述符设置一次性行为。epoll_wait 一次后，不会再 wait，除非使用 EPOLL_CTL_MOD 重新设置。
        * EPOLLWAKEUP: 设置此标记，使事件排队时系统保持唤醒。
            * 当系统设置了自动休眠模式 (/sys/power/autosleep) 时，为了保持设备唤醒直到事件处理完成，必须使用此标记。
        * EPOLLEXCLUSIVE：为附加到目标文件描述符 fd 的 epoll 文件描述符设置独占唤醒模式。【？？？】

## 3. epoll_wait
```c
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);
int epoll_pwait(int epfd, struct epoll_event *events,
               int maxevents, int timeout,
               const sigset_t *sigmask);
```
* 作用：等待 epoll 文件描述符上的 I/O 事件。
* 参数：
    * epfd：epoll 文件描述符；
    * events：等待的目标事件；
    * maxevents：返回的最大事件数量；
    * timeout：epoll_wait 阻塞超时，单位微秒。发生以下事件解除阻塞：
        * 监听的文件描述符有事件发出/传递；
        * 被信号处理函数中断；
        * 超时；
    * sigmask：屏蔽信号集？！【疑问】
* 返回：
    * 正常：就绪的文件描述符数量
    * 异常：-1，并设置 errno

## 4. 触发形式
* 水平触发：（默认是水平触发）
    * 触发条件：只要有`数据`可读/可写，即可触发。

* 边缘触发：（使用 EPOLLET 设置为边缘触发）
    * 触发条件：有`新数据`可读/可写，即可触发。
* 两种触发的不同在于：一次新数据，水平触发可能触发多次，边缘触发只会触发一次。

# epoll 的优点
select/poll的两个主要性能问题，就是epoll的优点：
* select/poll需要轮询查找就绪的描述符，epoll直接返回就绪的信息。
* 描述符集合需要从用户空间拷贝到内核空间。
    * epoll 的 add/remove 也是需要拷贝；
    * 对于就绪描述符，使用 mmap 进行内存共享，避免拷贝；

# A. 问题/拓展
* 可读：缓冲区非空，并且数据量超过读阈值。
* 可写：缓冲区未满。
* 什么时候使用 poll/select, 什么时候使用 epoll?
    * 满足以下几个条件，用 epoll，否则用 poll：
        * 使用的是 Linux 系统，并且有 epoll 系统调用；
        * 需要处理得是大的`活动描述符集`（至少 1000）；
        * 描述符集是相对稳定的。（因为 epoll 添加/删除描述符花销和 poll 一样很大，需要进入/离开内核空间）；
