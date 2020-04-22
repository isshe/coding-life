
[TOC]

Redis
---
Redis：REmote DIctionary Server（Redis），是一个key-value存储器，也是一个数据结构服务器(data structures server)。
Redis分为服务器(redis-server)和客户端(redis-cli)。

# 安装
略

# Redis 通用命令
* shell
```
sudo service redis-server restart
redis-cli
```

* redis-cli：
```
exist <key>             # 判断key是否存在
del <key>               # 删除某key
type <key>              # 获取key的类型
keys <key-pattern>      # 返回匹配的key列表
randomkey               # 随机返回一个存在的key，数据库空，则返回空字符串
clear                   # 情况命令界面
rename <old_key_name> <new_key_name>    # 重命名key
renamenx <old_key_name> <new_key_name>  # 新名字存在，则重命名失败
dbsize                  # 返回key的数量
expire <key> <seconds>  # 设置/更新key的超时时间，超时后删除
ttl <key>               # 查询剩余生存时间
flushdb                 # 清空当前数据库的key
flushall                # 清空所有数据库的key
config get <config_name>                # 获取配置
config set <config_name> <config_value> # 设置配置
auth                    # 认证
config resetstat        # 重置数据统计报告
info                    # 查询信息
- server：redis server信息
- client：client连接信息
- memory：内存占用信息
- persistence: RDB和AOF信息
- stats: 常规统计
- replication: master/slave信息
- cpu：CPU占用信息
- cluster: 集群信息
- keyspace: 数据库信息统计
- all：所有信息
- default: 常规设置信息
```


# Redis strings
Redis字符串是二进制安全的，这意味着Redis字符串能包含任意类型的数据。
但过大的文件不宜存入redis，一方面系统内存有限，另一个方面字符串类型最懂储存512字节的内容。

## Redis strings操作示例
```shell
set mykey somevalue     # 设置mykey，存在会替换
get mykey
set mykey newval nx     # nx：不存在才插入
set mykey newval xx     # xx：不存在也插入
set mykey abc ex 30     # 设置超时时间
set counter 100         # 初始化
incr counter            # +1
incrby counter 50       # +50
mset a 10 b 20 c 30     # 一次设置多个k-v
mget a b c              # 一次获取多个value
```

# Redis List
Redis List：字符串列表，最多能容纳`2^32-1`个元素。

## Redis List操作示例
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
Redis Hash：字符串字段与字符串值之间的映射。是展现对象的完美数据类型。

## Redis Hash操作示例
```
hmset <key> <sub_key1> <sub_value1> <sub_key2> <sub_value2> [...]
hget <key> <sub_key>
hgetall <key>
hmget <key> <sub_key1> <sub_key2> [...]
hincrby <key> <sub_key> <inc num>
```

# Redis 无序集合(set)
* 一个无序的字符串集合；
* 添加/删除/测试存在时间复杂度为O(1);
* 不允许重复元素；

## Redis无序集合操作示例
```
sadd myset 1 2 3 3 2 1  # 添加元素到集合
smembers myset          # 显示集合内所有元素
sismember myset 3       # 存在返回1
```

# Redis 有序集合(set)
* 有序；
* 添加/删除/更新时间复杂度O(N)；
* 不允许重复元素；
* 可根据权值(score)/次序(position)获取范围内的元素。

## Redis 有序集合操作示例
```
# zadd <key> <score> <value>
zadd zset 1 A           # 添加元素
zadd zset 10 J          # 添加元素
zadd zset 5 E           # 添加元素
zrange zset 0 -1        # 顺序输出
zrevrange zset 0 -1     # 逆序输出
zrange zset 0 -1 withscores     # 顺序连同score一起输出
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

## 发布/订阅
```
PUBLISH CHANNAL MESSAGE
SUBSCRIBE channel [channel ...] 
```

# 问题
## redis-cli中文乱码？
* 执行`redis-cli --raw`

## 什么是二进制安全？

## 如何获取所有key-value？
```
keys *  # 获取所有key
```

## 如何更新key的生存时间？
* expire指令既可以设置超时时间，也可以更新超时时间
```
expire <key> <seconds>
```
