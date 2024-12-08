# TCP 连接被重置（Reset）分析

> TODO：以下简略版，后续补充完整的案例/步骤。

## strace 方案

使用 strace 确认是否提前关闭 socket。注意 trace 不仅要 network 还要 close.

```
goal all {
    run {
        $ timeout 60 strace -tt -f -e trace=network,close -p 50795,
        nop;
    }
}
```

## stap 查看应用层调用栈


```
_probe _stap_probe("syscall.close")
{
    int fd = _stap_long("fd");
    _str sock = _stap_call_str_func("describe_fd", fd);
    printf("%s %ld\n", sock, _now_ms());
    _str bt = _sym_ubt(_ubt());
    printf("%s", bt);
}
```

## stap 查看内核调用栈

> https://access.redhat.com/solutions/1570493

```
# reset-monitor.stap begins here
#
# usage : stap --all-modules reset-monitor.stap

probe begin { printf ("Reset monitoring started...\n") }
probe end { printf("Reset monitoring stopped.\n") }

probe kernel.function("tcp_send_active_reset").call,
      kernel.function("tcp_v4_send_reset").call,
      module("ipt_REJECT").function("*").call
{
                        printf ("%s-%lu: %s(%d) called %s\n",
                        ctime(gettimeofday_s()), gettimeofday_ns(),
                        execname(), pid(), probefunc())
                        print_backtrace()
}
#
# reset-monitor.stap ends here
```
