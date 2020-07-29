
Redis 高端操作
---

# 主从服务器
## 建立主从服务器
* 在slave端执行
```
SLAVEOF IPADDRESS:PORT
如：SLAVEOF 127.0.0.1:6379
```


# 主从复制
主服务器：用于`写/读`操作的服务器。
从服务器：用于`读`操作的服务器。

## 主从复制过程
* `slave`与`master`建立连接，发送`psync`同步命令。
* `master`启动一个**后台进程**，将数据库快照保存到文件中，同时master**主进程**收集新的写命令，进行缓存。
* 后台完成保存后，将此文件发送给`slave`。
* slave将此文件保存到磁盘上。


## 主从复制特点
* 可以拥有多个slave。
* 多个slave可以连接其他slave，master宕机后，相连的slave转为master。
* 主从复制不会阻塞master，在同步数据时，master可以继续处理client请求。
* 提高了系统的可伸缩性。


# 事务处理
* redis-cli:
```shell
multi       # 进入事务模式
set name a  # 插入操作队列，此时还未操作
set name b
exec        # 此时执行
get name
```
**注意：redis事务执行出错后，不会恢复到之前的状态，因此不能用redis事务来保证数据的一致性**

# 持久化
redis的持久化方式：`snapshotting(快照)`和`Append-only file(AOF)`。
快照：将数据保存到文件中，默认方式。
AOF：将读写操作保存到文件中。


## 持久化操作示例
* 在配置文件中进行配置：
```
# 设置
save 900 1  # 900秒内如果超过1个key被修改，则保存快照。
save 300 10 # 300秒内如果超过10个key被修改，则保存快照。

appendonly yes      # 启用AOF持久化方式
appendsync always   # 收到写命令立即写入磁盘，性能最差，但保证了数据的完整性
appendsync everysec # 每秒写一次磁盘，在性能和持久化方面做了很好的折中
appendsync no       # 完全依赖OS，性能最好，持久化没有保证
```
* redis-cli
```
save        # 将数据写入磁盘
```


# 虚拟内存
> 虚拟内存的管理在`2.6`版本之后取消了，之后版本由redis进行管理。

## 虚拟内存配置
* redis.config:
```
vm-enable yes                   # 开启虚拟内存
vm-swap-file /tmp/redis.swap    # 虚拟内存路径
vm-max-memory 10000000          # 最大虚拟内存
vm-page-size 32                 # 页面大小
vm-pages 123217729              # 页面数
vm-max-threads 4                # 用于执行value对象换入工作的线程数量
```

# 集群
* 创建集群
```shell
redis-cli --cluster create 10.140.0.3:7001 10.140.0.3:7002 10.140.0.3:7003 10.140.0.3:7004 10.140.0.3:7005 10.140.0.3:7006 --cluster-replicas 1
```