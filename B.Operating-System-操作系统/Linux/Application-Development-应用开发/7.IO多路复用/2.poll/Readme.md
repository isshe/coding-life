[TOC]

# poll

# 相关函数
```c
#include <poll.h>

struct pollfd {
    int    fd;       /* file descriptor */
    short  events;   /* events to look for */
    short  revents;  /* events returned */
};

int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```
* 作用：类似于select。（指示内核等待多个事件，并在有一个或多个时间或经历指定时间后唤醒进程。）
* 参数：
    * fds[]: 指向一个结构数组第一个元素的指针。
    * nfds: 指定fds中的元素个数。
    * timeout: 
        * `INFTIM`： 永远等待；(INFTIM被定义为一个负值)
        * `0`：立即返回；
        * `> 0`: 等待指定数据的`毫秒`数。
* 返回：
    * 成功：就绪描述符数目
    * 超时：0
    * 出错：-1
* 数据结构说明：
    * events: 要测试的条件；
    * revents: 返回描述符的状态。
* events/revents标记：
|常值|说明|能作为evnets的输入吗？|能作为revents的输入吗？|
|---|---|---|---|
|POLLIN|普通或优先级带数据可读|+|+|
|POLLRDNORM|普通数据可读|+|+|
|POLLRDBAND|优先级带数据可读|+|+|
|POLLPRI|高优先级数据可读|+|+|
|POLLOUT|普通数据可写|+|+|
|POLLWRNORM|普通数据可写|+|+|
|POLLWRBAND|优先级带数据可写|+|+|
|POLLERR|发生错误||+|
|POLLHUP|发生挂起||+|
|POLLNVAL|描述符不是一个打开的文件||+|

# 数据分类
poll识别三类数据：普通(normal)、优先级带(priority band)、高优先级(high priority)
* 普通数据：
    * 所有正规TCP、UDP数据；
    * TCP连接读半部关闭时；（FIN）
    * TCP连接存在错误；（RST、超时）
    * 监听套接字上游新的连接可用；（多数实现为普通，也有实现为优先级带数据）
* 优先级带：
    * TCP的带外数据；
* 高优先级：
