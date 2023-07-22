# unexpected chunk number 0 (expected 1) for toast value 12599063 in pg_toast_16687

## 问题原因

数据损坏了，可能是相关文件被误删或者是硬盘损坏。

## 解决步骤

- 登录 PG：

```
psql -h 127.0.0.1 -p 5432 -U postgres -d DBNAME
```

`DBNAME` 为实际的数据库名称。

- 重新进行索引：

```
select reltoastrelid::regclass from pg_class where relname = 'TABLE_NAME';

REINDEX table TABLE_NAME;

REINDEX table pg_toast.pg_toast_TOASTID;

VACUUM analyze TABLE_NAME;
```

`TABLE_NAME` 为出问题的数据库表名称；`TOASTID` 为出问题的 toast id。


如无法修复，则可尝试备份数据库文件 COPY 出问题的表中的数据到另一张表，然后删除旧表，重命名新表：

```sql
-- 例如：
create table dupe_users as (select * from users);

drop table users;

alter table dupe_users RENAME TO users;
```
