# 打印指定进程当前调用栈

- 源码

```
probe process("/path/to/executable").function("*") {
    if (pid() == target()) {
        print_ubacktrace()
    }
}
```

- 执行

```
sudo stap -x 1234 trace.stp
```

- 指定 debuginfo（未验证）

```
export SYSTEMTAP_DEBUGINFO_PATH=/path/to/debuginfo
```
