#!/bin/bash

echo "[+] cgroup 类型（tmpfs 表示 v1，cgroup2fs 表示 v2）："
cgv=$(stat -fc %T /sys/fs/cgroup/)
echo "$cgv"

filename="memory.max"
if [ "$cgv" = "tmpfs" ]; then
    filename="memory.limit_in_bytes"
fi

echo "[+] 创建内存 cgroup ..."
mkdir /sys/fs/cgroup/memory/test

echo "[+] 将内存限制设置成 1MB ..."

echo 1048576 > /sys/fs/cgroup/memory/test/$filename

echo "[+] 将当前 shell 进程设置到 cgroup 中 ..."
echo $$ > /sys/fs/cgroup/memory/test/cgroup.procs

echo "[+] 当前内存状态："
cat /sys/fs/cgroup/memory/test/memory.stat | grep -E 'cache|rss'
cat /sys/fs/cgroup/memory/test/memory.usage_in_bytes

echo "[+] 访问一个文件 ..."
ls ./ > /dev/null
cat test-1.c > /dev/null

echo "[+] 当前内存状态："
cat /sys/fs/cgroup/memory/test/memory.stat | grep -E 'cache|rss'
cat /sys/fs/cgroup/memory/test/memory.usage_in_bytes

echo "[+] 运行程序申请 100KB 内存 ..."
./test1.out 102400 &

sleep 1

echo "[+] 当前内存状态："
cat /sys/fs/cgroup/memory/test/memory.stat | grep -E 'cache|rss'
cat /sys/fs/cgroup/memory/test/memory.usage_in_bytes
sleep 1

echo "[+] 运行程序申请 2MB 内存 ..."
./test1.out 2048000 &

sleep 1

echo "[+] 当前内存状态："
cat /sys/fs/cgroup/memory/test/memory.stat | grep -E 'cache|rss'
cat /sys/fs/cgroup/memory/test/memory.usage_in_bytes

echo "[+] 检查是否有 OOM Kill（不是必现） ..."
dmesg -T  | grep oom-kill | tail -n 1

sleep 2

echo "[+] 清理环境 ..."
# cgdelete memory:/test
# 将进程迁移回原来的 cgroup，再删除
echo $$ > /sys/fs/cgroup/memory/cgroup.procs
rmdir /sys/fs/cgroup/memory/test
