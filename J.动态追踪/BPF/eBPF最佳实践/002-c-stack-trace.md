# C 程序堆栈跟踪

```
bpftrace -e 'tracepoint:syscalls:sys_enter_openat /pid == 893/ { printf("%s %s\n", comm, str(args->filename)); }'
```

- 通过 `/.../` 来过滤——指定 pid 或者是程序名称
