BPFTrace
---

BPFTrace 是 BPF 高级跟踪语言。它的优势之一是提供了很多内置功能，无需自己实现。
BPFTrace 程序分为 3 部分：头部(header)、操作块(action block)、尾部(footer)。
* 头部：在加载 BPFTrace 时执行的特殊块，通常用来打印在输出顶部的信息，例如前言。可选。
* 尾部：在程序终止前执行的特殊块。可选。
* 操作块：指定要跟踪的探针的位置以及基于探针触发事件执行的操作。（至少要从有一个）


# 安装

见：https://github.com/iovisor/bpftrace/blob/master/INSTALL.md

# Hello World

> example.bt

```shell
BEGIN
{
    printf("starting BPFTrace program\n")
}

kprobe:do_sys_open
{
    printf("opening file descriptor: %s\n", str(arg1))
}

END
{
    printf("exiting BPFTrace program\n")
}
```

# 过滤

```shell
kprobe:do_sys_open /str(arg1) == "/etc/ld.so.cache"/
{
    printf("opening file descriptor: %s\n", str(arg1))
}
```

# 动态映射

```shell
kprobe:do_sys_open
{
    @opens[str(arg1)] = count()
}
```