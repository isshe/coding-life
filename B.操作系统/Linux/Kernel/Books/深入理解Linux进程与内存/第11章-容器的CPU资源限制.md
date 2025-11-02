# 第十一章 容器的 CPU 资源限制

## 问题

- 容器内使用 top 命令查看负载是准确的吗？为什么？
- 如何正确获取容器本身的 CPU 利用率？
- 是不是容器内 CPU 利用率只有 user 和 system 两项指标？如果是，那么为何？
- cgroup 的文件路径？
- cgroup v1 和 v2 的目录结构差异？
- 怎么判断是 v1 还是 v2？
- 如何找到容器关联的 cgroup？

## 总结

### 容器内使用 top 命令查看负载是准确的吗？为什么？

不。因为 top 命令的数据是根据 /proc/stat 得来，容器内默认不会单独挂载 /proc/stat，也就是用的宿主机的数据。

### 如何正确获取容器本身的 CPU 利用率？

TODO

### 是不是容器内 CPU 利用率只有 user 和 system 两项指标？如果是，那么为何？

不太准确：

1. 否：在容器内，也是可以看到 irq、softirq、nice 等指标，只是这些指标是**宿主机全局数据，不是专属于容器的**。
2. 是：cgroup 统计容器 CPU 是只有 user 和 system，其他指标被合并到这两个里面了。因此一些统计/监控工具只能显示这两个指标。

- user = user + nice
- system = system + irq + softirq

全局层面：中断独立统计（/proc/stat）
进程/容器层面：中断归属到触发者，合并到 system 时间（cgroup）

主要注意的是，cgroup 的内容，无论容器和宿主机 `cat /sys/fs/cgroup/cpu/cpuacct.stat` 都是只有 user 和 system，不要误解宿主机中会多出其他指标。

### cgroup 的文件路径？

```
/sys/fs/cgroup
```

### cgroup v1 和 v2 的目录结构差异？

V1 是多层级架构：每个控制器有独立的层级结构

```
├── cpu/              # CPU控制器独立目录
│   ├── cgroup.procs
│   ├── tasks
│   ├── cpu.shares
│   └── mygroup/
├── memory/           # 内存控制器独立目录
│   ├── cgroup.procs
│   ├── memory.limit_in_bytes
│   └── mygroup/
├── blkio/            # 块IO控制器独立目录
└── cpuset/           # CPU集控制器独立目录
```

V2 是统一层级架构：所有控制器共享一个层级

```
/sys/fs/cgroup/
├── cgroup.controllers        # 可用控制器列表
├── cgroup.subtree_control    # 启用的控制器
├── cgroup.procs              # 进程列表
├── cpu.max                   # CPU限制
├── memory.max                # 内存限制
├── io.max                    # IO限制
└── mygroup/
    ├── cgroup.controllers
    ├── cgroup.subtree_control
    ├── cgroup.procs
    ├── cpu.max
    └── memory.max
```

### 怎么判断是 v1 还是 v2？

```
mount | grep cgroup
```

查看挂载类型。

### 如何找到容器关联的 cgroup？

容器内 `cat /proc/1/cgroup | grep cpu | head -n 1 | awk -F ':' '{print $3}'`，再拼接上 `/sys/fs/cgroup/cpu` 即可。例如 `/sys/fs/cgroup/cpu/docker/52d139d279791f77654ed9aeb1bbd30b643bbfc6e11757bbb3f381a37816975d`
