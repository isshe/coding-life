# 第十一章 容器的 CPU 资源限制

## 问题

- 容器内使用 top 命令查看负载是准确的吗？为什么？
- 是不是容器内 CPU 利用率只有 user 和 system 两项指标？如果是，那么为何？
- cgroup 的文件路径？
- cgroup v1 和 v2 的目录结构差异？
- 怎么判断是 v1 还是 v2？
- 如何找到容器关联的 cgroup？
- account_process_tick 统计 CPU 处理时间时，为何要区分用户时间和系统时间？为何一次调用只会累计一种时间？
- struct cgroup 中的 rstat_cpu 是每个 CPU 都一个数组，还是每个 CPU 是数组中一个元素？
- cpu.stat 的时间统计怎么理解？只有每次 cat/read 的时候，才将各个 CPU 的用时聚合一下吗？为什么我 cat 宿主机中容器对应的 cpu.stat，nr_periods 等都显示 0？容器还在运行。
- 容器中的核是真正的 CPU 核吗？
- Linux 是如何对容器下的进程进行 CPU 限制的？
- 容器中的 CPU throttle 是什么？
- 为什么关注容器 CPU 性能时，除了关注利用率，还要关注 throttle 的次数和时间？
- 如何获取 throttle 的次数和时间？
- 容器中的 CPU 配额过小在性能上有没有问题？
- k8s 的 requests 和 limit 是什么含义？为什么 k8s 需要同时用两个字段来限制 CPU 的使用？
- 假如要申请一个 8 核的容器，应该设置 resources.requests.cpu，还是 resources.limits.cpu？如果需要同时设置，该如何设置？
- 如何正确获取容器中的 CPU 利用率？

## 总结

### 容器内使用 top 命令查看负载是准确的吗？为什么？

不。因为 top 命令的数据是根据 /proc/stat 得来，容器内默认不会单独挂载 /proc/stat，也就是用的宿主机的数据。

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

### account_process_tick 统计 CPU 处理时间时，为何要区分用户时间和系统时间？为何一次调用只会累计一种时间？

为了分别反映应用代码与系统调用之间的消耗。
因为一次 account_process_tick() 调用时，CPU 正在执行的状态是明确的单一状态：
- 如果此时 CPU 正在执行用户态指令（即 !in_interrupt() 且 user_tick == 1），就只累计到 user_time。
- 如果 CPU 正在执行内核代码（系统调用、软中断、内核线程），就累计到 system_time。
一次 tick 只有一个状态（user / system），因此只能增加其中一种时间。

### struct cgroup 中的 rstat_cpu 是每个 CPU 都一个数组，还是每个 CPU 是数组中一个元素？

定义

```c
// include/linux/cgroup-defs.h
struct cgroup {
    ...
    struct cgroup_rstat_cpu __percpu *rstat_cpu;
    ...
};
```

可以看到 rstat_cpu 是 `__percpu` 修饰的，也就是每个 CPU 有一个独立的 rstat_cpu 实例，每个 CPU 通过自己的 ID 拿到属于自己的那份，不保证连续，但逻辑上类似一个数组。
**结论：不是数组本身，而是一个 percpu 分配区，每个 CPU 拥有自己的那份数据。**
每个 CPU 只更新自己的计数，避免 cache line 争用；
读取 /sys/fs/cgroup/.../cpu.stat 时，才会从各 CPU 聚合汇总。

### cpu.stat 的时间统计怎么理解？只有每次 cat/read 的时候，才将各个 CPU 的用时聚合一下吗？为什么 cat 宿主机中容器对应的 cpu.stat，其中的 nr_periods 等都显示 0？容器还在运行。

> 只有每次 cat/read 的时候，才将各个 CPU 的用时聚合一下吗？
是的，只有读取 /sys/fs/cgroup/.../cpu.stat 时，才会从各 CPU 聚合汇总。
> 为什么 cat 宿主机中容器对应的 cpu.stat，其中的 nr_periods 等都显示 0？容器还在运行。
cat 得到的 nr_periods 都是 0，应该是因为宿主机同时用了 cgroup v1 和 v2。容器用的 cgroup v2，但是 cat 了 cgroup v1 的 cpu.stat。`nr_periods` 是 cgroup v1 中的内容；cgroup v1 中看到的应该是 `usage_usec` 等内容。

### 容器中的核是真正的 CPU 核吗？

不是。cgroup 底层不会为容器提供（绑定）任何独立的核，所谓的核是一个整体上允许该容器运行的**时间**。

### Linux 是如何对容器下的进程进行 CPU 限制的？

每个容器都有独立的 task_group 内核对象。通过修改 cpu.cfs_period_us、cpu.cfs_quota_us 或 cpu.max 等文件来添加限制，会写入到 task_group 中，内核会定时根据这个记录来给 task_group 分配时间。

### 容器中的 CPU throttle 是什么？

调度器在发现某个 task_group 的可用时间不足时，会将它从上级调度队列中摘除，不会再为这组进程分配 CPU。

### 为什么关注容器 CPU 性能时，除了关注利用率，还要关注 throttle 的次数和时间？

因为 throttle 会剥夺容器在一段时间内（一般一次 period_time 是 100ms）的 CPU 执行权。也就是如果前面 50ms 吧 CPU 额度用光，就要等待 50ms 才能再次得到执行。

### 如何获取 throttle 的次数和时间？

在 cgroup 的 cpu.stat 中可以获取。

### 容器中的 CPU 配额过小在性能上有没有问题？

CPU 配额过小会导致容器进程频繁被 throttled(限流),无法充分利用 CPU 时间，从而：
- 应用响应延迟增加
- 吞吐量下降
- 可能触发超时错误

感觉书中答案有问题，增大 CPU 配额主要解决的是"有足够时间运行",而不是"在哪个核上运行"的问题

> 书中答案：如果容器实例的 CPU 配额过小，会导致内核调度器当调度应用程序发生在核之间的迁移的时候，缓存命中率变低，单核性能下降。增大容器实例的 CPU 配额，这样即使调度时有核之间的迁移发生，也会有概率命中和自己共享同一地址空间的兄弟进程刚使用过的核，缓存还可以接着用。这样 CPU 核的运行性能会变高。

### k8s 的 requests 和 limit 是什么含义？为什么 k8s 需要同时用两个字段来限制 CPU 的使用？

requests 对应的就是按照权重（cpu.shares）进行分配，正常情况下可以作为 CPU 资源的下线保证，不管宿主机多忙，这个核数都有保证。但是也会有异常情况，例如机器故障或配置被修改，4 核 变 2 核。

**requests (相对权重，非硬保证)**:
- 映射到 `cpu.shares = requests * 1024`
- 在 CPU 竞争时，按 shares 比例分配时间片
- **关键**: 如果节点上所有 Pod 的 requests 总和 > 节点容量，每个 Pod 得到的可能低于自己的 requests。（按权重比例分配）
- 如果其他容器空闲，可以使用超过 requests 的资源 (受 limits 限制)

**limits (硬上限)**:
- 映射到 CFS quota: `cpu.cfs_quota_us = limits * 100000`
- 无论节点多空闲，都不能超过 limits
- 超过后被 throttled(限流),进程暂停等待下个周期

**为什么需要两个字段？**

1. **调度阶段**: requests 用于确保节点有足够容量
2. **运行阶段**:
   - requests 决定竞争时的相对优先级
   - limits 防止单个容器独占资源
3. **提高利用率**: 允许空闲时突破 requests，但不超过 limits

> 权重越大 vruntime 增长越慢，有更多的运行时间。基准权重是 1024。

### 假如要申请一个 8 核的容器，应该设置 resources.requests.cpu，还是 resources.limits.cpu？如果需要同时设置，该如何设置？

resources.limits.cpu.
取决于应用特性和具体需求 —— 希望获得什么 QoS 等级？

| QoS 类别 | 配置 | 特点 | 驱逐优先级 | 示例应用 |
|---------|------|------|----------| ------- |
| **Guaranteed** | requests = limits | 性能稳定，不被限流 | 最低 (最后被驱逐) | 数据库、缓存、金融交易系统 |
| **Burstable** | requests < limits | 可弹性扩展 | 中等 | Web 服务、API、微服务 |
| **BestEffort** | 不设置 | 完全抢占 | 最高 (最先被驱逐) | ETL（数据抽取 + 数据整理 + 数据加载）、数据分析、视频处理 |

### 如何正确获取容器中的 CPU 利用率？

- 通过 /sys/fs/cgroup/...文件计算：获取两次 CPU 使用时间的差值，再除以流逝的时间。
    - v1: /sys/fs/cgroup/cpu,cpuacct/<container_id>/cpuacct.usage
    - v2: /sys/fs/cgroup/<container_id>/cpu.stat
- 使用 lxcfs：利用率计算原理和前面一样，但是替换了容器内的 /proc/stat（默认容器中看到的也是宿主机的，替换以后就是容器自身的了）。
  - 实现原理：拦截容器内对 /proc、/sys/fs/cgroup 的访问，返回“容器视角”的虚拟资源信息。
