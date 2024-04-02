[TOC]

PostgreSQL
---

# select null 时使用默认值
```
COALESCE(y.released_id, 0)
```

# 类型转换数组
```sql
select ARRAY[row1] from table1;
```

# 把行聚合成数组
```sql
select col1, array_agg(col2)
from table1 group by col1;
```

# 去除聚合后的{NULL}
```
array_agg(distinct e.item) filter (where e.item is not null) as tcp_ports
```

# 查看表大小
```sql
SELECT
table_schema || '.' || table_name
AS table_full_name, pg_size_pretty(pg_total_relation_size('"' || table_schema || '"."' || table_name || '"')) AS size
FROM
information_schema.tables
ORDER BY
    pg_total_relation_size('"' || table_schema || '"."' || table_name || '"')
DESC limit 20;
```

# 查询语句性能分析
```
explain analyze select * from abc;
```

# 查看活动连接数量
```
select datname, usename, application_name, state, count(state) from pg_stat_activity group by datname, usename, application_name, state;
```

# 查看最大连接数量
```
show max_connections;
```

# 导出、导入 csv 数据

```sql
-- 导出
COPY (
    SELECT * FROM TABLE_NAME LIMIT 100 OFFSET 0
) To '/tmp/data.csv' WITH CSV DELIMITER ',' HEADER;

-- 导入
COPY TABLE_NAME FROM '/tmp/data.csv' DELIMITER ',' CSV HEADER;
```

# 查看从库状态

```
select * from pg_stat_replication;
```

# 不重启数据库生效访问控制 pg_hba.conf

```
SELECT pg_reload_conf();
```

# 打印数据库中所有表中 ID 列的最大值大于 10 的表

```sql
DO
$$
DECLARE
    _tbl text;
    _max_id bigint;
    _threshold bigint = 10000;
BEGIN
    FOR _tbl IN
        SELECT quote_ident(t.table_schema) || '.' || quote_ident(t.table_name)
        FROM information_schema.tables as t
        LEFT JOIN information_schema.columns as c
        ON t.table_schema = c.table_schema and t.table_name = c.table_name
        WHERE t.table_schema = 'public'
        AND t.table_type = 'BASE TABLE'
        AND c.column_name = 'id'
        AND c.data_type = 'integer'
    LOOP
        EXECUTE format('SELECT max(id) FROM %s', _tbl) INTO _max_id;
        IF _max_id > _threshold THEN
            RAISE NOTICE 'Maximum ID in table %: %', _tbl, _max_id;
        END IF;
    END LOOP;
END
$$;
```

方法二：

```sql
DO $$
DECLARE
    tableName text;
    columnExists boolean;
    maxId int;
BEGIN
    FOR tableName IN (SELECT table_name FROM information_schema.tables WHERE table_schema = 'public')
    LOOP
        EXECUTE format('SELECT EXISTS(SELECT 1 FROM information_schema.columns WHERE table_name = ''%s'' AND column_name = ''id'')', tableName) INTO columnExists;
        IF columnExists THEN
            EXECUTE format('SELECT max(id) FROM %s', tableName) INTO maxId;
            IF maxId > 100 THEN
                RAISE NOTICE 'Table %, max(id) = %', tableName, maxId;
            END IF;
        END IF;
    END LOOP;
END;
$$;
```

# 从 csv 文件中插入数据到指定表格中（冲突时更新）

```sql
-- 创建临时表
CREATE TEMP TABLE temp_test LIKE test;

-- 插入数据到临时表
/usr/local/openresty-postgresql12/bin/psql -U postgres -d or_edge_admin -c "\COPY temp_test FROM 'test.csv' WITH CSV HEADER"

-- 从临时表更新到目标表
INSERT INTO test
SELECT * FROM temp_test
ON CONFLICT (id) DO UPDATE
SET code = EXCLUDED.code;

-- 删除临时表
DROP TABLE temp_test;
```
