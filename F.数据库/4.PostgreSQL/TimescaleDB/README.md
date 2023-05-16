
TimescaleDB
---

# 常用

- 查询时序表大小：

```sql
select table_name, table_size, total_size
    from timescaledb_information.hypertable where table_schema='public';
```

- 查询时序库总大小：

```sql
select sum(pg_size_bytes(total_size)) from timescaledb_information.hypertable where total_size is not null;
```

- 增加清理策略：

```sql
select add_drop_chunks_policy('TABLE_NAME', INTERVAL '1 week', cascade_to_materializations => FALSE);
```

- 查询时序库清理策略：

```sql
-- 全部
select * from timescaledb_information.drop_chunks_policies;
-- 过滤
select * from timescaledb_information.drop_chunks_policies where hypertable = 'TABLE_NAME'::regclass;
```

- 调整时序库清理策略：

```sql
-- 删除清理策略
select remove_drop_chunks_policy('TABLE_NAME', if_exists => True);

-- 增加新的清理策略
select add_drop_chunks_policy('TABLE_NAME', INTERVAL '1 week', cascade_to_materializations => FALSE);

-- 修改 chunk 大小
SELECT set_chunk_time_interval('TABLE_NAME', INTERVAL '1 week');

-- 删除不再需要的 chunk （可选）
select drop_chunks(table_name =>'TABLE_NAME', older_than => INTERVAL '1 week', cascade_to_materializations => FALSE);
```


- 查询时序表 chunk 创建间隔

```sql
-- interval_length，单位是微秒
SELECT * FROM "_timescaledb_catalog"."dimension"

-- 转换成天数
SELECT *, (interval_length / 24 / 60 / 60 / 1000000) as interval  FROM "_timescaledb_catalog"."dimension";
```


- 更新时序表 chunk 创建间隔

```sql
SELECT set_chunk_time_interval('TABLE_NAME', interval '24 hours');
```
