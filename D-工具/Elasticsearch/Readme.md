


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