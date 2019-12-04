
[TOC]

Redis
---
Redis：REmote DIctionary Server（Redis），是一个key-value存储器，也是一个数据结构服务器(data structures server)。
Redis分为服务器(redis-server)和客户端(redis-cli)。

# 安装
略

# Redis strings
Redis字符串是二进制安全的，这意味着Redis字符串能包含任意类型的数据。
但过大的文件不宜存入redis，一方面系统内存有限，另一个方面字符串类型最懂储存512字节的内容。

## Redis字符串操作
```shell
set mykey somevalue     # 设置mykey，存在会替换
get mykey
set mykey newval nx     # nx：不存在才插入
set mykey newval xx     # xx：不存在也插入
set counter 100         # 初始化
incr counter            # +1
incrby counter 50       # +50
mset a 10 b 20 c 30     # 一次设置多个k-v
mget a b c              # 一次获取多个value
```

# Redis List
Redis List：字符串列表，最多能容纳`2^32-1`个元素。

## Redis列表操作
```shell
rpush mylist A B        # 右边插入
lpush mylist C          # 左边插入
lrange mylist 0 -1      # 查看这个列表
lpop mylist             # 左边弹出
rpop mylist             # 右边弹出
brpop mylist 10         # 右边弹出，数据不存在就阻塞指定时间等待数据，等不到返回nil
blpop mylist 10         # 左边弹出，数据不存在就阻塞指定时间等待数据，等不到返回nil
del mylist              # 删除列表
```

# Redis Hash
Redis Hash：字符串字段与字符串值之间的映射。
```
```


# 密码
## 设置密码
* config set
```
config set requirepass [password]
```
* 修改`redis.conf`
```shell
requirepass [password]
# 修改后重启服务
sudo service redis-server restart
```

## 设置主服务器密码
```
config set masterauth <password>
```

## 认证
* redis-cli
```shell
# 方法一
redis-cli
info    # 这里会提示没认证
auth [password]
info    # 正常显示
exit

# 方法二
redis-cli -a [password]
```



# 主从服务器
## 建立主从服务器
* 在slave端执行
```
SLAVEOF IPADDRESS:PORT
如：SLAVEOF 127.0.0.1:6379
```


# 主从复制
主服务器：用于`写`操作的服务器。
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

# 问题
## redis-cli中文乱码？
* 执行`redis-cli --raw`

## 什么是二进制安全？

## 如何获取所有key-value？
```
keys *  # 获取所有key
```
