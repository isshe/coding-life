[TOC]

# renice

**简介**：修改**正在运行**的进程的 nice 值。

## 语法格式

```bash
renice [-n] <priority> [-p|--pid] <pid>...
renice [-n] <priority>  -g|--pgrp <pgid>...
renice [-n] <priority>  -u|--user <user>...
```

## 常用选项

| 选项 | 长选项 | 说明 |
|------|--------|------|
| -n   | --priority <N> | 设置 nice 为 N |
| -p   | --pid | 将 nice 值设置给指定进程 |
| -g   | --pgrp | 将 nice 值设置给指定进程组 |
| -u   | --user | 将 nice 值设置给指定用户（用户名或用户 ID） |

## 使用示例

### 基础用法

设置成指定的 nice：

```
renice -n 0 {pid}
```

加/减指定 nice：

```bash
renice +5 {pid}
```

## 相关命令

- `nice` - 也是设置 nice 值。详见[renice](./nice.md)

## 学习记录

**掌握程度**：⭐⭐ (1-5 星)
**学习日期**：2025-09-06

## 参考资料

- `man renice` - 官方手册
