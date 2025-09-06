[TOC]

# chrt

**简介**：查看或修改进程的调度策略、调度优先级。

## 语法格式

```bash
# 修改策略
chrt [options] <priority> <command> [<arg>...]
chrt [options] --pid <priority> <pid>

# 获取策略
chrt [options] -p <pid>
```

## 常用选项

| 选项 | 长选项 | 说明 |
|------|--------|------|
| -b   | --batch  | 设置策略成 SCHED_BATCH |
| -d   | --deadline  | 设置策略成 SCHED_DEADLINE |
| -f   | --fifo  | 设置策略成 SCHED_FIFO |
| -i   | --idle  | 设置策略成 SCHED_IDLE |
| -o   | --other  | 设置策略成 SCHED_OTHER |
| -r   | --rr  | 设置策略成 SCHED_RR |

TODO 更多选项

## 使用示例

### 基础用法

获取进程的调度策略和优先级：

```bash
chrt -p 12345
```

获取各个调度策略的最大最小优先级：

```bash
chrt --max
```

设置进程的调度策略和优先级：

```bash
# 设置成完全公平策略（SCHED_OTHER）、优先级是 0
chrt -p -o 0 12345
```

## 注意事项

1. `-o` 等选项后面，要跟优先级，不然策略不会设置成功。

## 相关命令

- `nice` - 设置进程的 nice 值。
- `taskset` - 给进程绑定 CPU（设置进程的调度亲缘性）。

## 学习记录

**掌握程度**：⭐⭐ (1-5 星)
**学习日期**：2025-09-06

## 参考资料

- `man chrt` - 官方手册

