# 打印指定进程当前调用栈

- 源码

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
/usr/bin/stap trace.stp -m trace -p 4 -v
/usr/bin/staprun "trace.ko"
```

- 指定 debuginfo（未验证）

```
export SYSTEMTAP_DEBUGINFO_PATH=/path/to/debuginfo
```
