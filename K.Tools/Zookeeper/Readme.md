[TOC]

ZooKeeper
---


# 什么是ZooKeeper？
* ZooKeeper是一种`分布式协调服务`。
  * 在分布式系统中共享配置，协调锁资源，提供命名服务等。
* 设计目标是：
  * 提供简单的编程接口。
* Zookeeper 就是一个文件系统 + 监听通知机制。

# ZooKeeper的作用是什么？
* 润滑剂.

# 端口
* 2888: zk 间的通讯端口.
* 3888: 投票端口
* 2181: 服务端口

# ZooKeeper有哪些应用场景？
* 分布式锁。
* 服务注册和发现。
* 共享配置和状态信息。

# ZooKeeper如何使用？

# ZooKeeper的数据模型是怎样的？
* 树形。(类似 linux)

# ZooKeeper支持哪些API？
* `create`：在树中的某个位置创建一个节点
* `delete`: 删除节点。
* `exists`: 测试节点是否存在于某个位置。
* `get data`: 从节点读取数据。
* `set data`: 写入数据到节点。
* `get childen`: 获取节点的子节点列表。
* `sync`: 等待数据被传播。

# ZooKeeper保证：
* 顺序一致性：来自客户端的更新会按照顺序应用。
* 原子性：更新成功或失败，没有中间结果。
* 单一系统映像：无论客户端连接到哪个服务器，看到的都是一样的服务视图。
* 可靠性：一旦一个更新被应用后，将持续到被下一个更新覆盖。
* 及时性：确保系统的客户视图在特定时间范围内是最新的。

# znode 类型？
* 持久节点
* 持久有序节点
* 临时节点
* 临时有序节点
临时：当存储的客户端断开服务连接时，节点将被删除。
有序：给节点添加序号。

# 什么是 zk 的监听机制？
* 客户端可以监听 znode 节点。
* znode 改变时，会通知当前正在监听的客户端。

# zk 角色
* Leader: master主节点
* Follower: 从节点,有投票权利
* Observer: 从节点,没有投票权利
* Looking: 正在找 Leader 节点.

# zk 集群?
* 有主从之分
* 必须有 master 节点
* master: 读写操作
* slava: 只能读操作; 写操作来时,通知 master 节点.

# 选举策略?
* 数据新的优先选择.
  * zk 会给每个数据分配一个全局唯一的 zxid,因此 zxid 越大,数据越新.
  * zk 在写数据时,每个节点都会有一个自己的 FIFO 队列, 保证数据的有序.
* myid 大的优先选择.
  * 每个 zk 服务都会被分配一个全局唯一的数字 myid.

# 拓展
* `ZAB协议`？
  * ZooKeeper Atomic Broadcast。