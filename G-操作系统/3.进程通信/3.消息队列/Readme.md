[TOC]

# 消息队列
* **在新的应用程序中，不应再使用消息队列。**
    * 详见[此Readme 2.4节](../Readme.md)
    * 若需要客户进程和服务器进程之间的双向数据流，用`UNIX域套接字`或`全双工管道`。
* 队列：消息队列。信息的链接表，存储在内核中，由标识符标识。
* 队列ID：消息队列标识符。
* 与队列关联的数据结构：定义队列的`当前`状态
```c
struct msqid_ds {
    struct ipc_perm msg_perm;
    msgqnum_t       msg_qnum;       // 队列里的消息长度/数量
    msglen_t        msg_qbytes;     // 队列中能容纳的最大字节数
    pid_t           msg_lspid;      // 最后msgsnd()的pid
    pid_t           msg_lrpid;      // 最后msgrcv()的pid
    time_t          msg_stime;      // 最后msgsnd()的时间
    time_t          msg_rtime;      // 最后msgrcv()的时间
    time_t          msg_ctime;      // 最后改变时间
}
```

## 1. 消息队列的系统限制
![消息队列的系统限制](./msg_queue_system_limit.png)
* 导出的：这种限制来源于其他限制。
    * 如：Linux系统中，最大消息数受限于最大队列数，最大队列数受限于系统安装的RAM大小/数量。
    
## 2. 相关函数
* key转换标识符的规则，见[Readme](../Readme.md)
* 创建新队列、引用现有队列。
```c
#include <sys/msg.h>

// key: 键
// 返回：标识符/队列ID
int msgget(key_t key, int flag);
```

* 队列控制函数
```c
#include <sys/msg.h>
// cmd:
// * IPC_STAT: 取队列的smqid_ds结构，存放在buf中
// * IPC_SET: 将buf中的msg_perm.uid/gid/mode/msg_qbytes等复制到队列对应的msqid_ds结构中。
// * IPC_RMID: 删除队列及其中的所有数据，删除立即生效。
int msgctl(int msqid, int cmd, struct msqid_ds *buf)
```

* 放数据到队列中
```c
#include <sys/msg.h>

/*
 * msqid: 队列标识
 * ptr: 指向类似以下的结构
 *      * struct my_msg {
 *          long my_type;           // 数据类型【？？？】
 *          char my_test[nbytes];   // 数据
 *      }
 * nbytes: 数据大小
 * flag:
 *  * IPC_NOWAIT: 队列满，返回-1，errno = EAGAIN
 *    * 否则，阻塞到
 *      * 等待到队列空；
 *      * 队列被删除, 返回-1, errno = EIDRM；
 *      * 收到信号，并从信号处理程序返回；返回-1, errno = EINTR；
 */
int msgsnd(int msqid, const void *ptr, size_t nbytes, int flag);
```

* 从队列中取数据
```c
#include <sys/msg.h>

/*
 * type: 指定想要消息的类型。
 *  * type == 0: 队列中的第一个消息；
 *  * type > 0 : 队列中第一个类型为type的消息；
 *  * type < 0 : 返回队列中消息类型值<=`type绝对值`的消息。如果多个，则返回类型值最小的[第一个]消息。
 * flag:
 *  * IPC_NOWAIT: 队列空，立即返回-1, errno = ENOMSG;
 *    * 否则，阻塞到
 *      * 等待到队列有需要的数据；
 *      * 队列被删除, 返回-1, errno = EIDRM；
 *      * 收到信号，并从信号处理程序返回；返回-1, errno = EINTR；
 */
int msgrcv(int msqid, const void *ptr, size_t nbytes, long type, int flag);
```

## A. 参考
* 《UNIX环境高级编程 第三版》