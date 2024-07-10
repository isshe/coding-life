# 如何在 PG 中获取硬盘空间？

SELECT pg_size_pretty(pg_tablespace_size('pg_default')) AS default_tablespace_size;
SELECT spcname, pg_tablespace_location(oid), pg_size_pretty(pg_tablespace_size(oid)) FROM pg_tablespace;

SELECT * FROM pg_catalog.pg_ls_dir('/var/postgres12/data');
select setting from pg_settings where name='data_directory'
SELECT * FROM pg_catalog.pg_stat_file('/var/postgres12/data')


CREATE OR REPLACE FUNCTION sys_df() RETURNS SETOF text[]
LANGUAGE plpgsql AS $$
BEGIN
    CREATE TEMP TABLE IF NOT EXISTS tmp_sys_df (content text) ON COMMIT DROP;
    COPY tmp_sys_df FROM PROGRAM 'df | tail -n +2';
    RETURN QUERY SELECT regexp_split_to_array(content, '\s+') FROM tmp_sys_df;
END;
$$;

CREATE OR REPLACE FUNCTION sys_df(path text) RETURNS SETOF text[]
LANGUAGE plpgsql AS $$
DECLARE
    cmd text;
BEGIN
    CREATE TEMP TABLE IF NOT EXISTS tmp_sys_df (content text) ON COMMIT DROP;
    cmd := format('df -hB1 --output=source,size,used,avail,pcent,target %s | tail -n +2', path);
    EXECUTE format('COPY tmp_sys_df FROM PROGRAM %L', cmd);
    RETURN QUERY SELECT regexp_split_to_array(content, '\s+') FROM tmp_sys_df;
END;
$$;


    SELECT table_name, total_size, table_size, index_size, toast_size
    FROM (
        SELECT *, total_size-index_size-COALESCE(toast_size,0) AS table_size FROM (
            SELECT c.oid, nspname AS table_schema, relname AS table_name
                    ,c.reltuples AS row_estimate
                    ,pg_total_relation_size(c.oid) AS total_size
                    ,pg_indexes_size(c.oid) AS index_size
                    ,pg_total_relation_size(reltoastrelid) AS toast_size
            FROM pg_class c
            LEFT JOIN pg_namespace n ON n.oid = c.relnamespace
            WHERE relkind = 'r'
        ) a
    ) a
    WHERE table_schema = 'public' and table_name ~ 'errlog'
    ORDER BY total_size DESC;


SELECT pg_size_pretty(pg_total_relation_size('errlog'));

SELECT pg_size_pretty(pg_total_relation_size('errlog_tsdb')), pg_size_pretty(pg_indexes_size('errlog_tsdb')), pg_size_pretty(pg_toast_total_relation_size('errlog_tsdb'));

SELECT
    pg_size_pretty(pg_table_size('errlog_tsdb')) as table_size,
    pg_size_pretty(pg_indexes_size('errlog_tsdb')) as index_size,
    pg_size_pretty(pg_total_relation_size('errlog_tsdb') -
                   pg_table_size('errlog_tsdb') -
                   pg_indexes_size('errlog_tsdb')) as toast_size;

select table_name, table_schema, pg_size_pretty(pg_size_bytes(total_size)) as total_size
from timescaledb_information.hypertable
where total_size is not null and table_name ~ 'errlog_tsdb'  order by total_size desc;


SELECT pg_size_pretty(
    pg_total_relation_size('errlog_tsdb') +
    COALESCE(sum(pg_total_relation_size(c.oid)), 0)
) AS total_size
FROM pg_class c
JOIN pg_inherits i ON c.oid = i.inhrelid
WHERE i.inhparent = 'errlog_tsdb'::regclass;


WITH base_sizes AS (
    SELECT c.oid, nspname AS table_schema, relname AS table_name,
           c.reltuples AS row_estimate,
           pg_total_relation_size(c.oid) AS base_total_size,
           pg_indexes_size(c.oid) AS index_size,
           pg_total_relation_size(reltoastrelid) AS toast_size
    FROM pg_class c
    LEFT JOIN pg_namespace n ON n.oid = c.relnamespace
    WHERE relkind = 'r'
),
timescaledb_sizes AS (
    SELECT i.inhparent AS parent_oid,
           SUM(pg_total_relation_size(c.oid)) AS chunks_size
    FROM pg_class c
    JOIN pg_inherits i ON c.oid = i.inhrelid
    GROUP BY i.inhparent
)
SELECT bs.table_name,
       CASE
           WHEN ts.chunks_size IS NOT NULL THEN bs.base_total_size + ts.chunks_size
           ELSE bs.base_total_size
       END AS total_size,
       CASE
           WHEN ts.chunks_size IS NOT NULL THEN bs.base_total_size + ts.chunks_size - bs.index_size - COALESCE(bs.toast_size, 0)
           ELSE bs.base_total_size - bs.index_size - COALESCE(bs.toast_size, 0)
       END AS table_size,
       bs.index_size,
       bs.toast_size
FROM base_sizes bs
LEFT JOIN timescaledb_sizes ts ON bs.oid = ts.parent_oid
WHERE bs.table_schema = 'public' and bs.table_name ~ 'errlog'
ORDER BY total_size DESC;
