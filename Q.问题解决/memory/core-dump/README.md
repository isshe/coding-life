# Core Dump

- 生成 core 文件

```bash
# 查看配置
ulimit -a

# 设置不限制 core 文件大小
ulimit -c unlimited
```

如果 `ulimit -a` 中 core 文件大小显示 0，则表示不开启 core 文件。

- 查看 core 文件

```bash
# 查看 core_pattern
cat /proc/sys/kernel/core_pattern

# 如果使用的是 systemd-coredump，则是以下目录
ls /var/lib/systemd/coredump/

# 解压 zst 文件
zstd -d xxx.zst
# 或
zstd -d xxx.zst -o xxx.core
```

- 如果使用 systemd-coredump 但是没有找到文件，则修改 core_pattern 后重新生成

```bash
echo core-%e | sudo tee /proc/sys/kernel/core_pattern
```
