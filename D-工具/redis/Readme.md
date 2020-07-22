
[TOC]

Redis
---
Redis：REmote DIctionary Server（Redis），是一个key-value存储器，也是一个数据结构服务器(data structures server)。
Redis分为服务器(redis-server)和客户端(redis-cli)。
特性:
* 基于内存运行，性能高效
* 支持分布式，理论上可以无限扩展
* key-value存储系统
* 开源的使用ANSI C语言编写、遵守BSD协议、支持网络、可基于内存亦可持久化的日志型、Key-Value数据库，并提供多种语言的API

参考文档:
http://redisdoc.com/
http://www.redis.cn/commands.html

# NoSQL
非关系型数据库.
分类:
* Key-Value: Redis
* 文档型: MongoDB, ElasticSearch
* 面向列: Hbase, Cassandra
* 图形化: Neo4j


# 安装
略

# Redis 通用命令
* shell
```
sudo service redis-server restart
redis-cli
```

* redis-cli：
```shell
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
# 设置及获取
set key value
get key

# 批量设置/获取
mset key value [key value ...]
mget key [key ...]

# 自增/自减 1
incr key
decr key

#  自增/自减 x
incrby key x
decrby key x

# 不存在则设置
setnx key value
set key value nx


# 设置超时时间
set key value ex seconds     # 设置超时时间
setex key seconds value

# 追加
append key value

# 获取长度
strlen key
```

# Redis List
Redis List：字符串列表，最多能容纳`2^32-1`个元素。

## Redis List操作示例
```shell
# 插入数据到列表
lpush key value [value ...]
rpush key value [value ...]

# 插入数据到列表: key 不存在或 key 不是列表, 则什么都不做.
lpushx key value
rpushx key value

# 修改: key[index] = value
lset key index value

# 弹出数据
lpop key
rpop key

# 获取列表指定范围数据
lrange key min max
#lrange mylist 0 -1

# 获取指定位置的数据
lindex key index

# 获取列表长度
llen key

# 删除数据:  删除 key 中 count 个 value.
# count = 0: 所有value
# count > 0: 左到右删除 count 个 value
# count < 0: 右到左删除 count 个 value
lrem key count value

# 保留 indexx 到 indexy 的数据, 包含 indexx 及 indexy
ltrim key indexx indexy

# 从list1 右边 pop, 插入到 list2 左边.
rpoplpush list1 list2
```

# Redis Hash
Redis Hash：字符串字段与字符串值之间的映射。是展现对象的完美数据类型。

## Redis Hash操作示例
```shell
# 获取/设置
hset key field value
hget key field

# 批量获取/设置
hmset key field value [field value ...]
hmget key field [field ...]

# 自增(无 hdecrby)
hincrby key field x

# 不存在则插入
hsetnx key filed value

# 判断是否存在
hexist key field

# 删除一个或多个 field
hdel key field [field ...]

# 获取所有
hgetall key

# 获取所有 filed
hkeys key

# 获取所有 value
hvalues key
```

# Redis 无序集合(set)
* 一个无序的字符串集合；
* 添加/删除/测试存在时间复杂度为O(1);
* 不允许重复元素；

## Redis无序集合操作示例
```shell
# 加入数据到集合
sadd key value [value ...]

# 获取所有数据
smembers key

# 判断是否在集合中
sismember key value

# 随机弹出 count 个数据
spop key [count]

# 交集
sinter set1 set2 ...

# 并集
sunion set1 set2 ...

# 差集
sdiff set1 set2 ...

# 删除数据
srem key value [value ...]
```

# Redis 有序集合(set)
* 有序；
* 添加/删除/更新时间复杂度O(N)；
* 不允许重复元素；
* 可根据权值(score)/次序(position)获取范围内的元素。

## Redis 有序集合操作示例
```shell
# 添加数据
zadd key score value [score value ...]

# 增加key[value]的分数
zincrby key score value

# 获取分数
zscore key value

# 获取 set 大小
zcard key

#  获取分数范围的数据(包含)
zcount key scoremin scoremax

# 删除
zrem key value [value ...]

# score升序(小到大)排序,返回指定范围的数据
# withscore: 分数也一起返回
zrange key indexx indexy [withscore]

# score降序(大到小)排序,返回指定范围的数据
zrevrange key indexx indexy [withscore]

# 根据分数范围获取数据
# limit 限制获取的数量
zrangebyscore key scoremin scoremax [withscore] [limit offset count]
```

# key 相关操作
```shell
# 根据匹配模式获取 key
keys pattern
# keys *

# 设置 key 的生存时间: 能活多久
expire key seconds
pexpire key milliseconds

# 设置 key 的生存时间: 能活到什么时候
# 1970~
expireat key timestamp
pexpireat key timestamp

# 剩余生存时间
ttl key
pttl key

# 移除生存时间
# 1: 移除成功
# 0: key 不存在生存时间或 key 不存在
persist key
```

# 数据库相关操作
```shell
# 选择库
select 0~15
# 移动 key 到 db
move key db
# 清空当前库
flushdb
# 清空所有库
flushall
# 获取库大小(key 的数量)
dbsize
# 查看最后一次操作时间
lastsave
# 监控器: 实时监控redis接口到的操作
monitor
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

## 发布/订阅
```
PUBLISH CHANNAL MESSAGE
SUBSCRIBE channel [channel ...] 
```

# 问题
## redis-cli中文乱码？
* 执行`redis-cli --raw`

## 什么是二进制安全？

## redis 的优缺点?