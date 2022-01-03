BPFTool
---


# 安装

```shell
git clone git@github.com:torvalds/linux.git
git checkout v5.4
cd tools/bpf/bpftool
make && make install
```

# 使用

## 特征查看

查看当前系统所支持的所有特征。

```shell
bpftool feature
```

## 启用 jit

```shell
cat /proc/sys/net/core/bpf_jit_enable
echo 1 > /proc/sys/net/core/bpf_jit_enable
```

## 获取系统中已运行的 BPF 程序

```shell
bpftool prog show
bpftool prog show id <ID>
bpftool prog show id <ID> --json
bpftool prog dump xlated id <ID>
bpftool prog dump xlated id <ID> visual &> output.out
# sudo apt install graphviz
dot -Tpng output.out -o visual-graph.png
```

## 启用 BPF 统计信息

```shell
sysctl -w kernel.bpf_stats_enabled=1
```

## 载入并持久化 BPF 程序到 BPF 文件系统

```shell
bpftool proc load bpf_proc.o /sys/fs/bpf/bpf_proc
```

## 检查 BPF 映射

```shell
bpftool map show
```

## 创建 BPF 映射

```shell
bpftool map create /sys/fs/bpf/counter type array key 4 value 4 entries 5 name counter
```

## 更新 BPF 映射

```shell
bpftool map update id 1 key 1 0 0 0 value 1 0 0 0
```

## 查看 BPF 映射

```shell
bpftool map dump id 1
```

## 将映射附加到程序上

```shell
# 使用名称
bpftool prog load bpf_prog.o /sys/fs/bpf/bpf_prog \
    map name counter /sys/fs/bpf/counter

# 使用索引
bpftool prog load bpf_prog.o /sys/fs/bpf/bpf_prog \
    map idx 1 /sys/fs/bpf/counter
```

## 查看附加到特定接口的程序

```shell
bpftool perf show
bpftool net show
bpftool cgroup tree
```