[TOC]

Zookeeper
---

# 查询节点
> ls [路径]

# 查询数据
> get [路径]

# 添加/创建节点
> create -s -e [路径] [数据]
`-s`: 有序
`-e`: 临时

# 修改节点
> set [路径] [数据]

# 删除节点
> delete [路径]
> rmr [路径]

# 查看 zk 状态
> ./zkServer.sh status