[TOC]

PostgreSQL
---

# select null时使用默认值
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

# 查看表大小
```sql
SELECT
table_schema || '.' || table_name
AS table_full_name, pg_size_pretty(pg_total_relation_size('"' ||table_schema || '"."' || table_name || '"')) AS size
FROM
information_schema.tables
ORDER BY
    pg_total_relation_size('"' || table_schema || '"."' || table_name || '"')
DESC limit 20;
```