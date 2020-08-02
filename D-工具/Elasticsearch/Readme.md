
[TOC]

ElasticSearch
---

# 前言
Elasticsearch 是一个分布式、可扩展、实时的搜索与数据分析引擎。基于 lucene 实现。

# 倒排索引

# 字段类型
官方文档：https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping-types.html#_core_datatypes

## 字符串类型
* text：可被分词，一般用于全文检索
* keywork：不可被分词
## 数值类型
* long：
* integer
* short
* byte
* double：
* float：
* half_float：精度比 float 小一半。
* scaled_float: 
## 时间类型
* date：
* date_nanos: Data nanoseconds
## 布尔类型
* boolean
## 二进制类型
* binary：暂时支持 Base64 编码字符串
## 范围类型
* integer_range: 整型范围
* float_range: 浮点范围:
* long_range: 长整型范围
* double_range: 浮点范围
* date_range: 时间范围
* ip_range: ip 范围

## 经纬度类型
* geo_point: 用于存储经纬度
* 使用场景：
    * 距离 xxx 多少多少米。

## ip 类型
* ip：可以存储 IPv4 或 IPv6 地址。

# 分页
## from + size
限制：两者之和不能超过 10000.
### 查询原理
* 对查询内容进行分词
* 将词汇去分词库中查询文档 ID
* 去各个分片中拉取指定的文档
* 将文档数据根据 score 进行排序
* 根据 from 的值，舍弃不需要的数据
* 返回结果

## 深分页 scroll
限制：不适合做实时的查询，因为相关数据存在上下文中（内存）。
### 查询原理
* 对查询内容进行分词
* 将词汇去分词库中查询文档 ID
* 将文档 ID 存放在一个 ES 的上下文中
* 根据指定的 size 个数，去 ES 中检索指定个数的数据；拿完数据的文档 ID，会在上下文中被删除。
* 如需要下一页，直接取 ES 的上下文中找后续的内容。
* 循环上面两步。

# 其他
```
# 获取模板
curl -XGET http://host:port/_template/barad
curl -XGET http://host:port/_template/attacklog

# 设置模板
```