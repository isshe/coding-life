
[TOC]

# Posix 信息队列
* 消息队列可认为是一个消息链表。具有`随内核的持续性`
* 有读权限的线程可以从队列中取走信息；
* 有写权限的线程可以从队列中放置信息；
* 允许异步事件通知。

## 1. 相关函数
```c
#include <mqueue.h>
// 打开消息队列
mqd_t mq_open(const char *name, int oflag, ...
                /* mode_t mode, struct mq_attr *attr */);
// 关闭消息队列，调用进程不用，但不会被删除消息队列
int mq_close(mqd_t mqdes);

// 从系统中删除消息队列
// mq_unlink(const char *name);

// 
int mq_notify(mqd_t mqdes, const struct sigevent *notification);
```


## A. Posix 消息队列和 SystemV 消息队列的区别
* Posix 消息队列的`读`总是返回最高优先级的最早消息；System V 消息队列的`读`可以返回任意指定优先级的消息。
* 往`空队列`放置一个消息时，Posix 消息队列允许产生一个信号或者启动一个线程。System V 没有类似的机制。