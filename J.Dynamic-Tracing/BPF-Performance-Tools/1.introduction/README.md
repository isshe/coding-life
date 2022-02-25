介绍
---

# 1.8 初识 bpftrace：跟踪 open()

```shell
bpftrace -e 'tracepoint:syscalls:sys_enter_open { printf("%s %s\n", comm, str(args->filename)); }'
```
