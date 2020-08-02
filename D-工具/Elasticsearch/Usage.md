
[TOC]

Elasticsearch 使用
---

# 1. 索引

## 1.1 创建索引
```json
PUT /persion
{
  "settings": {
    "number_of_shards": 5, # 分片数量
    "number_of_replicas": 1 # 副本数量
  }
}
```

## 1.2 指定类型
```json
PUT /book
{
  "settings": {
    "number_of_shards": 5,
    "number_of_replicas": 1
  },
  "mappings": {
    "properties": {
      "name": { # 字段名
        "type": "text" # 字段类型
        "analyzer": "ik_max_word", # 指定分词器
        "index": true,  # 如果为 false，则不能被检索
        "store": false # 是否需要额外存储一份
      }
    }
  }
}
```

## 1.3 查看索引信息
```json
GET /persion
```

## 1.4 删除索引
```json
DELETE /persion
```

## 1.5 索引模板
> TODO 

# 2. 文档
_index、_type、_id 三个内容确定一个文档。
> 7.0版本以后, _type 默认为 _doc。

## 2.1 创建文档
### 自动生成 ID
```json
POST /book/_doc
{
  "name": "天才小当家",
  "author": "heshushen",
  "count": 100000,
  "onsale": "2020-08-01"
}
```

### 指定 ID
```json
POST /book/_doc/id132
{
  "name": "天才小当家",
  "author": "heshushen",
  "count": 100000,
  "onsale": "2020-08-01"
}
```

## 2.2 修改文档
### 覆盖修改
```json
POST /book/_doc/id132
{
  "name": "天才大当家",
  "author": "heshushen",
  "count": 100000,
  "onsale": "2020-08-01"
}
```

### 2.3 指定修改
```json
# 标记废弃
POST /book/_doc/id123/_update
{
  "doc": {
    "name": "天才大当家"
  }
}
# 新版用这个
POST /book/_update/id123
{
  "doc": {
    "count": 12345
  }
}
```

## 2.4 删除文档
```json
DELETE /book/_doc/3jDSqHMB_dj2Cr9HsgbG
```

## 2.5 查询文档

### term查询
完全匹配的查询，搜索之前不会对搜索的关键字进行分词。
```json
# 弃用
POST /book/<type>/_search
# 新版使用
POST /book/_search
{
  "from": 0,
  "size": 10,
  "query": {
      "term": {
        "name": {
          "value": "天才小当家"
      }
    }
  }
}
```
* from: 开始位置
* size：查询数量
**from + size 的限制：两者之和不能超过 10000.**

### terms 查询
和 term 查询机制一样。
terms 在针对一个字段包含多个值的时候使用
term: where key=value
terms: where key=value1 or key=value2 or key=value3
```json
POST /book/_search
{
  "query": {
      "terms": {
        "author": ["heshu", "heshushen"]
    }
  }
}
```

### match 查询
match 查询属于高层查询。会根据查询的字段类型不用，采用不同的查询方式。
* 查询日期或数值：会将你基于字符串查询内容转换为对应类型。
* 查询内容是不能被分词的内容（keyword），match 查询不会将查询类型进行分词。
* 查询内容是可分词内容（text），match 查询会先分词再查询。
match 查询实际低层就是多个 term 查询。


### match_all查询所有
```json
GET /book/_search
{
  "query": {
    "match_all": {}
  }
}
```

### match 查询
```json
POST /book/_search
{
  "query": {
      "match": {
        "name": "小当家"
    }
  }
}
```

### 布尔 match 查询
```json
POST /book/_search
{
  "query": {
    "match": {
      "name": {
        "query": "大当家 天才",
        "operator": "or"  # and
      }
    }
  }
}
```

### multi_match 查询
match 针对一个 field 进行查询；multi_match 对多个 field 进行检索。
```json
POST /book/_search
{
  "query": {
    "multi_match": {
      "query": "大当家", 
      "fields": ["name", "author"]
    }
  }
}
```

### 其他查询
### id 查询
```shell
GET /book/_doc/id123
```

### ids 查询
根据多个 id 查询，类似 where id in (id1, id2, ...)
```json
POST /book/_search
{
  "query": {
    "ids": {
      "values": ["id123", "123"]
    }
  }
}
```

### prefix 查询
前缀查询，通过一个关键字指定一个 field 的前缀，进行查询。
使用场景：在线搜索提示。（根据下面的查询结果来看，也不是这种场景）
prefix 查询较为低效，使用模糊搜索、正则匹配更高效。
参考文档：https://www.elastic.co/guide/cn/elasticsearch/guide/current/prefix-query.html
```json
# 查询结果不符合预期！
POST /book/_search
{
  "query": {
    "prefix": {
      "name": "天才小"
    }
  }
}

POST /book/_search
{
  "query": {
    "prefix": {
      "name": {
        "value": "天才小"
      }
    }
  }
}
```

### fuzzy 查询
模糊查询。可以用错别字。但是查询结果不稳定。
```json
POST /book/_search
{
  "query": {
    "fuzzy": {
      "author": {
        "value": "hesheshen",
        "prefix_length": 4
      }
    }
  }
}
```
* prefix_length: 不允许出错的字符长度

### wildcard 查询
通配查询: 通配符`*`，占位符`?`。
```json
POST /book/_search
{
  "query": {
    "wildcard": {
      "author": {
        "value": "hesh?sh*"
      }
    }
  }
}
```

### range 查询
范围查询：只针对数值类型。
```json
POST /book/_search
{
  "query": {
    "range": {
      "count": {
        "gt": 12346,
        "lt": 100001
      }
    }
  }
}
```

### regexp 查询
正则查询：通过正则表达式进行匹配。
**PS: prefix, fuzzy, whidcard, regexp查询效率比较低。**
```json
POST /book/_search
{
  "query": {
    "regexp": {
      "author": "he.*"
    }
  }
}
```

### scroll 查询
```json
# 首次查询，会创建_scroll_id，后面的查询使用。
POST /book/_search?scroll=1m
{
  "query": {
    "match_all": {}
  },
  "size": 1,
  "sort": [
    {
      "count": {
        "order": "desc"
      }
    }
  ]
}

# 后续的查询
POST /_search/scroll
{
  "scroll_id": "FGluY2x1ZGVfY29udGV4dF91dWlkDnF1ZXJ5VGhlbkZldGNoBRRmREE3cVhNQl9kajJDcjlIQXhMRQAAAAAAAnT8FmFKeEJZZnNhUVZtbk1vYTFEMFU1SmcUZmpBN3FYTUJfZGoyQ3I5SEF4TEYAAAAAAAJ0_hZhSnhCWWZzYVFWbW5Nb2ExRDBVNUpnFGZ6QTdxWE1CX2RqMkNyOUhBeExGAAAAAAACdP8WYUp4Qllmc2FRVm1uTW9hMUQwVTVKZxRnREE3cVhNQl9kajJDcjlIQXhMRgAAAAAAAnUAFmFKeEJZZnNhUVZtbk1vYTFEMFU1SmcUZlRBN3FYTUJfZGoyQ3I5SEF4TEUAAAAAAAJ0_RZhSnhCWWZzYVFWbW5Nb2ExRDBVNUpn",
  "scroll": "1m"
}

# 删除 scroll 在 ES 上下文的记录
# DELETE /_search/scroll/scroll_id
DELETE /_search/scroll/FGluY2x1ZGVfY29udGV4dF91dWlkDnF1ZXJ5VGhlbkZldGNoBRRmREE3cVhNQl9kajJDcjlIQXhMRQAAAAAAAnT8FmFKeEJZZnNhUVZtbk1vYTFEMFU1SmcUZmpBN3FYTUJfZGoyQ3I5SEF4TEYAAAAAAAJ0_hZhSnhCWWZzYVFWbW5Nb2ExRDBVNUpnFGZ6QTdxWE1CX2RqMkNyOUhBeExGAAAAAAACdP8WYUp4Qllmc2FRVm1uTW9hMUQwVTVKZxRnREE3cVhNQl9kajJDcjlIQXhMRgAAAAAAAnUAFmFKeEJZZnNhUVZtbk1vYTFEMFU1SmcUZlRBN3FYTUJfZGoyQ3I5SEF4TEUAAAAAAAJ0_RZhSnhCWWZzYVFWbW5Nb2ExRDBVNUpn

```
* size：分页大小
* sort：排序方式
    * 根据 count 字段进行降序排序。
* scroll：指定存储时间

## 2.6 查询后删除
> 待补充: https://www.bilibili.com/video/BV1Qz411e7yx?p=33

