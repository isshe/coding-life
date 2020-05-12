[TOC]

用法
---

# 启动服务
```shell
# 启动zookeeper
/usr/local/kafka/bin/zookeeper-server-start.sh /usr/local/kafka/config/zookeeper.properties

# 启动kafka
/usr/local/kafka/bin/kafka-server-start.sh /usr/local/kafka/config/server.properties
```

# 创建Topic
```shell
/usr/local/kafka/bin/kaftopics.sh --create \
--zookeeper localhost:2181 --replication-factor 1 \--partitions 1 --topic test
```

# 生产消息
```shell
/usr/local/kafka/bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test
```

# 消费消息
```shell
# 新版
/usr/local/kafka/bin/kafka-console-consumer.sh \
--bootstrap-server localhost:9092 --topic test --from-beginning

# 旧版
/usr/local/kafka/bin/kafka-console-consumer.sh \
--zookeeper localhost:2181 --topic test --from-beginning
```