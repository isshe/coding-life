# 打印指定进程当前调用栈

目的：

- 了解如何获取当前进程的调用栈。
- 了解如何把地址转换成函数名称。
- 了解如何生成火焰图。

## 获取运行中的进程的调用栈

```
probe perf.type(1).config(0).sample(10) {
    try {
        // do something
        printf("User backtrace:\n%s\n", sprint_ubacktrace());
    } catch (err) {
        warn(sprintf("catch error: %s", err));
    }
}

probe process("/path/to/executable").begin {
    warn("process begin...");
}

probe process("/path/to/executable").end {
    warn("process end...");
}

probe timer.s(1) {
    warn("timer...");
    abort(); // or exit();
}

probe begin {
    warn("start tracing...");
}

probe end {
    warn("stop tracing...");
}
```

- 执行

```
sudo stap -x 1234 trace.stp
```

- 编译后执行

```bash
# 生成 trace.ko
/usr/bin/stap trace.stp \
    -d OBJECT:MELF:DEBUG:DWZ \
    -d OBJECT:MELF:DEBUG \
    -d OBJECT:MELF \
    -m trace -p 4 -v -k

# 执行
/usr/bin/staprun "trace.ko"
```

`-d` 选项指定可执行文件、melf 文件、debug 文件、dwz 文件。melf 信息通常在可执行文件中。
debug 文件 和 dwz 文件可以从 dbgsym 的 deb 包或 rpm 的 debuginfo 包中获取（详见 [0-find-and-install-debug-info.sh](0-find-and-install-debug-info.sh)）。

**如果系统上安装了 debuginfo 文件，可以直接使用 `--ldd` 选项代替 `-d` 选项指定调试信息，systemtap 会自动搜索调试信息。**

具体执行步骤见：[1-run-and-trace.sh](1-run-and-trace.sh)

## 生成火焰图

- 下载 FlameGraph 仓库

```bash
git clone git@github.com:brendangregg/FlameGraph.git
```

- 生成火焰图

```bash
cd FlameGraph
./stackcollapse-stap.pl stap.out | ./flamegraph.pl > out.svg
```

实例详见 [2-gen-flame-graph.sh](2-gen-flame-graph.sh)

## 实例

```bash
bash 1-run-and-trace-stap.sh
bash 2-gen-flame-graph.sh
```

## 总结

1. 如何获取当前进程的调用栈？

    使用 `sprint_ubacktrace()` 或 `sprint_backtrace()` 等函数来获取。详见示例 [trace.stp](trace.stp)。

2. 如何把地址转换成函数名称？

    当 stap 能找到 debuginfo 时，输出的堆栈会自动转换成名称，而不是地址。可使用 `-d` 选项指定需要 debuginfo 的可执行文件。

3. 如何生成火焰图？

    详见 [生成火焰图](#生成火焰图) 中的步骤。
