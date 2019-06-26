[TOC]

# kqueue
内核事件通知机制：kernel event notification mechanism

## 1. kqueue
```c
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

int kqueue(void);
```
* 作用：分配一个kqueue文件描述符。
* 参数：无
* 返回：
    * 正常：kqueue文件描述符
    * 异常：-1, errno

## 2. kevent/kevent64
```c
struct kevent {
    uintptr_t       ident;          /* 事件标识 */
    int16_t         filter;         /* 筛选器标识：用于处理此事件的过滤器。 */
    uint16_t        flags;          /* 事件操作标记：要对事件执行的操作。 */
    uint32_t        fflags;         /* 过滤器标记：指定过滤器操作 */
    intptr_t        data;           /* 过滤器数据 */
    void            *udata;         /* 用户自定义数据 */
};

struct kevent64_s {
    uint64_t        ident;          /* 事件标识 */
    int16_t         filter;         /* 筛选器标识：用于处理此事件的过滤器。 */
    uint16_t        flags;          /* 事件操作标记：要对事件执行的操作。 */
    uint32_t        fflags;         /* 过滤器标记：指定过滤器操作 */
    int64_t         data;           /* 过滤器数据 */
    uint64_t        udata;          /* 用户自定义数据 */
    uint64_t        ext[2];         /* 过滤器拓展：此字段存储事件过滤器的扩展名。 什么类型的扩展取决于使用何种类型的过滤器。*/
};

struct kevent_qos_s {
    uint64_t        ident;          /* 事件标识 */
    int16_t         filter;         /* 筛选器标识：用于处理此事件的过滤器。 */
    uint16_t        flags;          /* 事件操作标记：要对事件执行的操作。 */
    uint32_t        qos;            /* quality of service when servicing event */
    uint64_t        udata;          /* 用户自定义数据 */
    uint32_t        fflags;         /* 过滤器标记：指定过滤器操作 */
    uint32_t        xflags;         /* 拓展过滤器标记 */
    int64_t         data;           /* 过滤器数据 */
    uint64_t        ext[4];         /* 过滤器拓展：QoS变体为特定于过滤器的用途提供两倍的扩展值。*/
};

int kevent(int kq, const struct kevent *changelist, int nchanges,
    struct kevent *eventlist, int nevents,
    const struct timespec *timeout);

int kevent64(int kq, const struct kevent64_s *changelist, int nchanges,
    struct kevent64_s *eventlist, int nevents, unsigned int flags,
    const struct timespec *timeout);

int kevent_qos(int kq, const struct kevent_qos_s *changelist, int nchanges,
    struct kevent_qos_s *eventlist, int nevents, void *data_out,
    size_t *data_available, unsigned int flags);
```
* 作用：向队列注册感兴趣的事件，并返回感兴趣的待处理事件。
* 参数：
    * kq: kqueue文件描述符；
    * changelist: 指向`events/events64_s/kevent_qos_s`结构数组的指针。
    * nchanges: changelist的大小。
    * eventlist: 指向`events/events64_s/kevent_qos_s`结构数组的指针。
    * nevnets: 
    * flags: 
    * timeout: 超时时间。
    * data_out: 
    * data_available: 
* 返回：


## 3. EV_SET/EV_SET64/EV_SET_QOS
```c
EV_SET(&kev, ident, filter, flags, fflags, data, udata);

EV_SET64(&kev, ident, filter, flags, fflags, data, udata, ext[0], ext[1]);

EV_SET_QOS(&kev, ident, filter, flags, qos, udata, fflags, xflags, data, ext[0], ext[1], ext[2], ext[3]);
```


## A. 问题/拓展
