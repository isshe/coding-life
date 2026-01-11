#!/bin/bash

echo "[+] cgroup 类型（tmpfs 表示 v1，cgroup2fs 表示 v2）："
stat -fc %T /sys/fs/cgroup/
cgv=$(stat -fc %T /sys/fs/cgroup/)

filename="memory.max"
if [ "$cgv" = "tmpfs" ]; then
    filename="memory.limit_in_bytes"
fi

echo "[+] 创建内存 cgroup ..."
mkdir /sys/fs/cgroup/memory/test

echo "[+] 将内存限制设置成 1 字节 ..."
echo 1 > /sys/fs/cgroup/memory/test/$filename

echo "[+] 实际最小值："
cat /sys/fs/cgroup/memory/test/$filename

echo "[+] 将内存限制设置不限制 ..."
echo -1 > /sys/fs/cgroup/memory/test/$filename

echo "[+] 实际最小值："
cat /sys/fs/cgroup/memory/test/$filename

echo "[+] 清理环境 ..."
# cgdelete memory:/test
# 将进程迁移回原来的 cgroup，再删除
echo $$ > /sys/fs/cgroup/memory/cgroup.procs
rmdir /sys/fs/cgroup/memory/test
