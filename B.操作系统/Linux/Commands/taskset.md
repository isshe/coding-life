[TOC]

# taskset

**简介**：获取/设置进程的调度亲缘性。

## 语法格式

```bash
taskset [options] [mask | cpu-list] [pid|cmd [args...]]
```

## 常用选项

| 选项 | 长选项 | 说明 |
|------|--------|------|
| -a   | --all-tasks  | 针对指定 PID 的所有任务（线程）进行操作 |
| -p   | --pid | 针对指定的已存在 PID 进行操作 |
| -c   | --cpu-list | 以列表格式显示和指定 CPU|

## 使用示例

### 基础用法

获取指定 PID 的亲缘性：

```bash
taskset -p 700
```

绑定指定 PID（700）到指定 CPU：

```bash
taskset -pc 0,3,7-11 700
```

## 相关命令

- `nice` - 设置进程的 nice 值。详见 [nice 命令](./nice.md)
- `renice` - 设置进程的 nice 值。详见 [renice 命令](./renice.md)
- `chrt` - 查看或修改进程的调度策略、调度优先级。详见 [chrt 命令](./chrt.md)

## 学习记录

**掌握程度**：⭐⭐⭐ (1-5 星)

**学习日期**：2025-09-07

## 参考资料

- `man taskset` - 官方手册

