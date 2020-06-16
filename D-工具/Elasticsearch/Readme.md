


# 查询数量
```
curl -XGET 'http://localhost:9200/foo/_count"
```

# 查询所有
```
curl -XGET 'http://localhost:9200/foo/_search?pretty=true&size=50' -d '
{
    "query" : {
        "match_all" : {}
    }
}'
```

# 其他
```
# 获取模板
curl -XGET http://host:port/_template/barad
curl -XGET http://host:port/_template/attacklog

# 设置模板
```