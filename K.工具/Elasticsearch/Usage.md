
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
> 7.0 版本以后，_type 默认为 _doc。

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

### term 查询
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


### match_all 查询所有
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
通配查询：通配符`*`，占位符`?`。
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
**PS: prefix, fuzzy, whidcard, regexp 查询效率比较低。**
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
根据 term/match 等查询方式去删除大量的文档。
> 如果需要删除的内容，是大部分数据，推荐是创建一个新的 index，然后把数据添加到新的 index。

```
POST /book/_delete_by_query
{
  "query": {
    "range": {
      "count": {
        "gt": 12345,
        "lt": 100001
      }
    }
  }
}
```

## 2.7 复合查询
### 2.7.1 bool 查询
复合过滤器，将你的多个查询条件，以一定的逻辑组合在一起。
* must：and 的意思，全部都需要匹配。
* must_not: 全部都不能匹配。
* should: or 的意思。
```json
POST /book/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "range": {
            "count": {
              "gte": 12356,
              "lte": 1000010
            }
          }
        },
        {
          "term": {
            "author": {
              "value": "isshe"
            }
          }
        }
      ],
      "must_not": [
        {
          "match": {
            "name": "天才大当家"
          }
        }
      ]
    }
  }
}
```

## 2.8 boosting 查询
boosting 查询可以影响查询后的 score。
* positive：只有匹配上 positive 的内容，才会被放到结果集中。
* negative：如果匹配上和 positive 并且匹配上 negative，就可以降低这样的文档的分数。
* negative_boost：指定系数，必须小于 1.0. 
关于查询结果分数计算：
* 搜索的关键字在文档中出现的频次越高，分数越高。
* 指定的文档内容越短，分数越高。
* 被分词的关键字的词汇，被分词库匹配的数量越多，分数越高。
```json
POST /book/_search
{
  "query": {
    "boosting": {
      "positive": {
        "match": {
          "name": "大当家"
        }
      },
      "negative": {
        "match": {
          "author": "isshe"
        }
      },
      "negative_boost": 0.2
    }
  }
}
```

## 2.9 filter 查询
query 查询：根据查询条件去计算文档的匹配度得到一个分数，并根据分数进行排序。
filter 查询：根据查询条件去查询文档，但**不计算分数**，并且会对经常被过滤的数据进行缓存。
```json
POST /book/_search
{
  "query": {
    "bool": {
      "filter": [
        {
          "term": {
            "author": "isshe"
          }
        },
        {
          "prefix": {
            "name": "天才"
          }
        }
      ]
    }
  }
}
```

## 2.10 高亮查询
对查询的内容以一定的特殊样式展示给用户。
* fregment_size: 指定高亮数据展示多少个字符回来。默认 100.
* pre_tags：指定前缀标签。
* post_tags：指定后缀标签。
highlight 和 query 同级别。

```json
POST /book/_search
{
  "query": {
    "bool": {
      "filter": [
        {
          "term": {
            "author": "isshe"
          }
        },
        {
          "prefix": {
            "name": "天才"
          }
        }
      ]
    }
  },
  "highlight": {
    "fields": {
      "name": {}
    },
    "pre_tags": [
      "<font color='red'>"
    ],
    "post_tags": [
      "</font>"
    ]
  }
}
```

## 2.11 聚合查询

### 2.11.1 去重计数查询
去重计数 Cardinality：第一步对指定的 field 进行去重，第二步进行统计。
```json
POST /book/_search
{
  "aggs": {
    "res": {  # 结果名称
      "cardinality": {  # 聚合类型
        "field": "author"
      }
    }
  }
}
```

### 2.11.2 范围统计
统计一定范围内出现的文档个数。
* from: `>=`
* to: `<`
```json
POST /book/_search
{
  "aggs": {
    "res": {
      "range": {
        "field": "count",
        "ranges": [
          {
            "from": 12345,
            "to": 100001
          }
        ]
      },
      "ip_range": {
        "field": "ip",
        "ranges": [
          {
            "from": "10.0.0.5",
            "to": "10.0.0.10"
          }
        ]
      },
      "date_range": {
        "field": "date",
        "format": "MM-yyy", # 可以指定格式
        "ranges": [
          {
            "from": "now-10d/d",
            "to": "now"
          }
        ]
      }
    }
  }
}
```
* range: range/ip_range/date_range

### 2.11.3 统计聚合查询
统计字段的最大值、最小值、平均值。
```json
POST /book/_search
{
  "aggs": {
    "res": {
      "extended_stats": {
        "field": "count"
      }
    }
  }
}
```

## 2.12 经纬度查询
* geo_distance: 直线距离检索方式
* geo_bounding_box: 以两个点 (左上右下) 确定一个矩形范围，获取其中所有数据。
* geo_polygon: 以多个点确定一个多边形，获取其中所有数据。
```json
# geo_distance
POST /map/_search
{
  "query": {
    "geo_distance": {
      "location": {
        "lon":116.401969,
        "lat": 39.914492
      },
      "distance": 10000,
      "distance_type": "arc"
    }
  }
}

# geo_bounding_box
POST /map/_search
{
  "query": {
    "geo_bounding_box": {
      "location": {
        "top_left": {
          "lon": 116.401969,
          "lat": 39.914492
        },
        "bottom_right": {
          "lon": 116.503154,
          "lat": 39.873749
        }
      }
    }
  }
}

# geo_polygon
POST /map/_search
{
  "query": {
    "geo_polygon": {
      "location": {
        "points": [{
          "lon": 116.401969,
          "lat": 39.914492
        },
        {
          "lon": 116.503154,
          "lat": 39.873749
        },
        {
          "lon": 116.32838,
          "lat": 39.899881
        }
        ]
      }
    }
  }
}

```
* distance：指定距离
* distance_type：指定形状



