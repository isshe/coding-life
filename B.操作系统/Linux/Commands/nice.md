[TOC]

# nice

**简介**：设置进程的 nice 值，干预进程的调度。

## 语法格式

```bash
nice [OPTION] [COMMAND [ARG]...]
```

## 常用选项

| 选项 | 长选项 | 说明 |
|------|--------|------|
| -n   | --adjustment=N  | 给 nice 值加 N，注意不是设置成 N，是加 N |

## 使用示例

### 基础用法

**启动前**设置 nice 值：

```bash
nice -n -20 vi
```

## 相关命令

- `renice` - 也是设置 nice 值。详见[renice](./renice.md)

## 学习记录

**掌握程度**：⭐⭐⭐ (1-5 星)

**学习日期**：2025-09-06

## 参考资料

- `man nice` - 官方手册
