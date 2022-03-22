# 第5章 bpftrace

bpftrace 是一款基于 BPF 和 BCC 的开源跟踪器。

参考指南：https://github.com/iovisor/bpftrace/blob/master/docs/reference_guide.md
bpftrace 一行教程：https://github.com/iovisor/bpftrace/blob/master/docs/tutorial_one_liners_chinese.md

## 5.2 探针类型

|类型|缩写|描述|
|---|---|---
| tracepoint | t | 内核静态插桩点 |
| usdt | U | 用户态静态定义插桩点 |
| kprobe | k | 内核动态两数插桩 |
| kretprobe | kr | 内核动态函数返回值插桩 |
| uprobe | u | 用户态动态函数插桩 |
| uretprobe | ur | 用户态动态函数返回值插桩 |
| software | s | 内核软件事件 |
| hardware | h | 硬件基于计数器的插桩, PMC |
| profile | p | 对全部 CPU 进行时间采样 |
| interval | i | 周期性报告（从一个CPU 上） |
| BEGIN | | bpirace 启动 |
| END | |bpftrace 退出 |

## 5.7 bpftrace 编程

示例：对内核函数 vfs_read() 的执行时间进行测量。

```bpftrace
#!/usr/1ocal/bin/bpftrace

// this program times vfs_read()

kprobe:vfs_read
{
    @start[tid] - nsecs;
}

// 这里过滤是为了确保只计算记录了开始时间的调用
kretprobe:vfs_read
/@start[tid]/
{
    $duration_us - (nsecs - @start[tid]) / 1000;
    @us = hist(duration_us);
    delete(@start[tid]);
}
```

### 5.7.1 用法

```shell
bpftrace -e program
bpftrace file.bt
./file.bt
```

### 5.7.2 程序结构

```bpftrace
probes /filter/ { actions }
probes /filter/ { actions }
...
```

### 5.7.3 注释

注释方式同 C 语言
```c
// 单行

/*
 * 多行
 */
```

### 5.7.4 探针格式

```c
// 以类型名字开始
type:identifier[:identifier2[...]]

// 示例：
kprbe:vfs_read
uprobe:/bin/bash/readline
```

### 5.7.10 变量

有 3 种类型的变量：内置变量、临时变量和映射表变量。

内置变量：由 bpftrace 预先定义，通常只读。如 pid，comm、nsecs、curtask。
临时变量：以 “$” 作为前缀，如 `$x = 1;`，只能在当前动作中使用。
映射表变量；以 “@” 作为前缀，可以跨动作使用。如 `@a = 1; @path[pid, $fd] = arg0`。


### 5.7.11 映射表函数

`@x = count();` 和 `@x++` 的区别：
- 前者类型是 count 类型的对象；后者是整数。
- 前者使用了每 CPU 独立的映射表；后者是一个全局映射表，非 CPU 独立。
    - 因此后者因为并发更新可能产生小无差。

## 5.9 bpftrace 的探针类型

> 见 5.2 表格

## 5.9.1 tracepoint

格式：

```c
// tracepoint_name 是跟踪点全名
// 使用内置变量 args 来访问跟踪点参数
tracepoint:tracepoint_name
```

示例：

- [获取进程创建情况](process_clone.bt)

```bpftrace
// process_clone.bt

tracepoint:syscalls:sys_enter_clone
{
    printf("-> clone() by %s PID %d\n", comm, pid);
}

tracepoint:syscalls:sys_exit_clone
{
    printf("<- clone() return %d, %s PID %d\n", args->ret, comm, pid);
}
```

### 5.9.2 usdt

格式

```c
// probe_name：二进制文件中 USDT 的探针名字
usdt:binary_path:probe_name
usdt:library_path:probe_name
usdt:binary_path:probe_namespace:probe_name
usdt:library_path:probe_namespace:probe_name
```

列出二进制文件中的所有探针：

```
bpftrace -l 'usdt:/usr/local/cpython/python'
```

### 5.9.3 kprobe 和 uretprobe

格式：

```
uprobe:binary_path:function_name
uprobe:binary_path:function_name
uretprobe:binary_path:function_name
uretprobe:binary_path:function_name
```

参数：

- uprobe：arg0, arg1, ..., argN，类型是 64 位无符号整型。如果是指针可以强制类型转换。
- uretprobe：retval，类型是无符号整型。
