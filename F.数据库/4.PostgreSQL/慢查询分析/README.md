[TOC]

Postgresql 慢查询语句记录与分析
---

# 1. 记录

> postgresql 以 12 版本为例

我们需要先打开 postgresql 的慢查询日志，此为前置条件，先记录下我们时间较长的查询。

* 编辑配置文件：`/var/postgres12/data/postgresql.conf`

```
#log_min_duration_statement = -1        # -1 is disabled, 0 logs all statements
                                        # and their durations, > 0 logs only
                                        # statements running at least this number
                                        # of milliseconds
```

修改 `-1` 为 `200`，表示大于等于 200 毫秒的操作将被记录到日志。

* 重载配置

```
/path/to/pg_ctl reload -D /path/to/pgdata
```

# 2. 获取慢查询语句

## 2.1 切 postgresql 超户

```
sudo su - postgres
```

## 2.2  获取慢查询语句

* 打开日志文件

```
less /path/to/pg_log/postgresql.log
```

* 跳到文件末尾

```
Shift + G
```

* 查询关键字 `duration`

因为被记录的日志格式形如：

```
duration xxx ms  select ….
```

# 3. 分析

* 连接 postgresql 服务器

```
path/to/psql -U postgres -d <database>
```

* 打开计时

```
\timing
```

* 语句分析

```
explain select max(id) as max_id from test_table where  created < now() - interval '24 hours';
                                                          QUERY PLAN
------------------------------------------------------------------------------------------------------------------------------
 Finalize Aggregate  (cost=12673.45..12673.46 rows=1 width=8)
   ->  Gather  (cost=12673.24..12673.45 rows=2 width=8)
         Workers Planned: 2
         ->  Partial Aggregate  (cost=11673.24..11673.25 rows=1 width=8)
               ->  Parallel Seq Scan on test_table  (cost=0.00..10631.80 rows=416575 width=8)
                     Filter: (created < (now() - '24:00:00'::interval))
(6 rows)
```

`explain` 是显示查询计划，不会真正执行，所以甚至 update 也可以放在 explain 后面。
但是要注意，不能是 `explain analyze` ，带了 `analyze` 就真跑了，所以这个不能用于 `DML` 。

> DQL: SELECT
> DML: UPDATE/DELETE/INSERT
> DDL: CREATE TABLE/VIEW/INDEX/SYN/CLUSTER
> DCL: GRANT/ROLLBACK/COMMIT

另外可以看到结果中显示的是 `Seq Scan`，顺序扫描，所以可以为它创建一个索引，变成 `Index Scan`。

```
create index test_index on test_table (created);
```

然后再次分析，如果时间没有明显减少，可以再去掉此索引。
重复以上步骤，即可逐步减少系统中的慢查询。
