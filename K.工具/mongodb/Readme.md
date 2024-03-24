
[TOC]
 
MongoDB
---

# 简介
* 非关系型数据库；
* 面向集合的存储；
* 角色包含：
  * 数据库：可以在创建文档时自动创建
  * 集合：可以在创建文档时自动创建
    * 一组文档的组合
  * 文档
    * 文档的逻辑联系
      * 嵌入式关系
      * 引用式关系
* 使用 bson 格式存储数据

# 公共命令
* shell
```shell
sudo service mongodb restart
mongo   # mongo shell
```

# 查询
## 查询数据库
```
show dbs
```

## 查询当前数据库
```
db
```

## 查询集合
```
use <dbname>
show collections
```

## 查询文档
### 普通查询
```
db.<collection_name>.find()                     # 特殊字符的话，就用下面的
db.<collection_name>.find().pretty()            # 更美观
db.getCollection("<collection_name>").find()
```

### AND/OR查询
```
db.<collection_name>.find({"field1": "value2", "filed2": "value2"})           # AND查询
db.<collection_name>.find({$or:[{"field1": "value2", "filed2": "value2"}]})   # OR查询，列表中每个字典是OR关系
```

### 比较查询
```
db.<collection_name>.find({"field":{$gt:xxx}})  # $gt, $lt, $gte, $lte, $ne
```
* `$gt`: 大于
* `$lt`: 小于
* `$gte`: 大于等于
* `$lte`: 小于等于
* `$ne`: 不等于

### 模糊查询
```
db.<collection_name>.find({"field":/^xxx/})     # 以xxx开头的
db.<collection_name>.find({"field":/xxx$/})     # 以xxx结尾的
```

### 类型查询
```
db.<collection_name>.find({"name":{$type:2}})
db.<collection_name>.find({"name":{$type:string}})
db['collection'].find()
```
* type：
  * 1：双精度 (Double)
  * 2：字符串 (String)
  * 3：对象 (Object)
  * 4：数组 (Array)
  * 5：二进制数据 (Binary data)
  * 7：对象 ID(Object id)
  * 8：布尔类型 (Boolean)
  * 9：日期 (Data)
  * 10：空 (Null)
  * 11：正则表达式 (Regular Expression)
  * 13：JS 代码 (JavaScript)
  * 14：符号 (Symbol)
  * 15：有作用域的 JS 代码 (JavaScript with scope)
  * 16:32 位整数 (32-bit integer)
  * 17：时间戳 (Timestamp)
  * 18:64 位整数 (64-bit integer)
  * -1：最小值 (Min key)
  * 127：最大值 (Max key)


### 指定数量查询
```
db.<collection_name>.find().limit(<number>)
```

### 跳过指定数量查询
```
db.<collection_name>.find().skip(<number>)
db.<collection_name>.find().limit(<number>).skip(<number>)
```


### 排序查询
```
db.<collection_name>.find().sort({"field":1|-1})   # 1:升序，-1:降序
```


# 创建
## 创建/切换数据库
```
use <dbname>
```

## 创建集合
```
use <dbname>
db.createCollection(<collection_name>, [options])
```
* options: 一个座位初始化的文档，可选。
  * capped：Boolean 类型，`true`则创建固定大小的集合，大小达到最大时，就覆盖之前的条目。
  * size: `capped`为`true`时，则需要指定，单位为`byte`。
  * max: 最大文档条目数。

## 创建文档/数据
* insert/save
  * insert: 不能插入相同的记录（主键为准）
  * save：可以插入新记录，也可以更新旧记录
```
use <dbname>
db.<collection_name>.insert([
    {"xxx": "yyy"}, {"aaa": "bbb"}
])
db.<collection>.insertOne()
db.<collection>.insertMany()
```
> collections_name: 如果集合不存在，会自动创建。


# 更新

## 更新文档
```
use mydb
db.<collection_name>.insert({"field":"value", ...})
# 如果没有指定 $set 会直接更新记录
db.<collection_name>.update({"field":"value", ...}, {$set: {"field1":"value1"}})    # 方法一
db.<collection_name>.save({"_id":"xxx", ...})                                       # 方法二
```


# 删除
## 删除数据库
```
use <dbname>
db.dropDatabase()
```

## 删除集合
```
use <dbname>
show collections
db.<collections_name>.drop()
db.getCollection("<collections_name>").drop();
db['<collection>'].drop()
```

## 删除文档
```
db.<collection_name>.remove({"field":"value"})  # 删除匹配的文档
```

# 索引
```
db.<collection_name>.ensureIndex({"field":1|-1}, [options])
示例：
db.users.ensureIndex({"name":1}, {background:1})
```
* options:
|参数 | 类型 | 默认值 | 概述|
|---|---|---|---|
|background|Boolean|false|不阻塞其他数据库操作|
|unique|Boolean|false|建立的索引是否唯一|
|name|string||索引的名称，未指定则自动生成|
|dropDups|Boolean|false|建立唯一索引时，是否删除重复记录|
|sparse|Boolean|false|对文档不存在的字段数据不启用索引|
|expireAfterSecond|integer||设置集合的生存时间，单位`秒`|
|v|index version||索引的版本号|
|weight|document||索引权重值|
|default-language|string|英语 | 默认语言|
|language-override|string|language||


# 聚合
```
db.<collection_name>.aggregate([{"$option1":{}}, {"$option2":xx}, ...])
```
* options:
  * `$project`: 修改输入文档结构。
  * `$match`: 过滤数据，只输出符合条件的文档。
  * `$limit`: 限制返回的文档数。
  * `$skip`: 跳过返回的文档数，返回余下的。
  * `$unwind`: 将文档中某一个数组类型字段拆分成多条，每条包含数组中的一个值。
  * `group`: 将集合中的文档分组，可用于统计结果。
  * `sort`: 将文档排序后输出。
  * `geoNear`: 输出接近某一地理位置的有序文档。

* 示例
```
db.user.aggregate([
        {$match:{user_id:{$gt:0,$lte2}}},
        {$group:{_id:"user_id", count:{$sum:1}}},
        {$limit:1}
    ])
```

# system
* 数据库的信息存储在集合中，他们同意使用系统的命名空间：`DBNAME.system.*`
```
<DBNAME>.system.namespaces: 列出所有名字空间
<DBNAME>.system.indexs: 列出所有索引
<DBNAME>.system.porfile: 列出数据库概要信息
<DBNAME>.system.users: 列出访问数据库的用户
<DBNAME>.system.sources: 列出服务器信息
```

# 备份/恢复
```
# 备份
./mongodump --host=mongodb-base.gaia-1.yf-1.chongqing.yf-1.tcepoc.fsphere.cn --port=27017 --db=cc_data -u user_0 -p ccdata

# 恢复
./mongorestore -h ${mongodb_host}:${mongodb_port} --username=${mongodb_user} --password=${mongodb_pass} -d ${mongodb_db} --dir=${mongodb_dir}
```