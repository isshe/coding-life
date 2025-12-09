[TOC]

# perf

**简介**：Linux 性能分析工具

## 语法格式

```bash
perf [--version] [--help] [OPTIONS] COMMAND [ARGS]
```

## 常用命令

- `perf list`: 查看当前环境支持的事件。
- `perf top`: 查看当前系统或进程的性能统计。
- `perf stat`: 当前系统或进程的指定事件统计。
- `perf sched`: 查看当前系统的调度器工作状况。
- `perf record`: 采样统计系统或进程的性能事件，输出到 perf.data 文件中。
- `perf report`: 读取分析 perf.data 文件，并显示分析结果。

## 使用示例

```bash
perf list -h
# 列出硬件事件
perf list hw cache pmu
# 列出软件事件
perf list sw
# 列出静态 tracepoint 事件
perf list tracepoint

per stat
# 统计指定可执行文件
perf stat </path/to/exec>
# 统计指定进程
perf stat -p <PID>
# 统计指定性能事件，硬件、软件、tracepoint 都行
perf stat -e instructions
# 统计多个事件
perf stat -e instructions,cache-misses

# 采集调度信息，将生成 perf.data
perf sched record sleep 5
# 采集指定进程，将生成 perf.data
perf sched record -p <PID> sleep 5
# 采集指定核心，将生成 perf.data
perf sched record -C 0,1 sleep 5
# 采集指定事件，将生成 perf.data
perf sched record -e sched:sched_switch sleep 5
# 分析：调度器延迟
perf sched latency --sort max
# 分析：显示所有采样信息
perf sched script

# 通用采集，将生成 perf.data
# 采集指定事件
perf record -e cache-misses sleep 5
# 记录调用栈
perf record -g sleep 5
# 每秒采集次数，每秒采样 100 次
perf record -F 100 sleep 5
# 采集间隔，每 100 次采集一次
perf record -c 100 sleep 5
# 采集指定 CPU
perf record -C 0,1 sleep 5
perf record -C 0-2 sleep 5

# 分析
perf report
# 分析结果排序
perf report --sort <key[,key2,...]>

# 实时统计
perf top
perf top -p <PID>
```
## 参考资料

- `man perf` - 官方手册
