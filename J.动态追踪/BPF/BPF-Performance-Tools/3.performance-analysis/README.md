# 第3章 性能分析

开始性能分析之前，先思考一些问题：

- 性能分析的目标是什么？优化用户体验？降低运行成本？
- 开展哪些工作对达到这个目标是有帮助的？

量化指标

- 延迟：多久可以完成一次请求或操作，通常以毫秒为单位。
- 速率：每秒操作或请求的速率。
- 吞吐量：通常指每秒传输的数据量，以比特 （bit）或者字节（byte）为单位。
- 利用率：以百分比形式表示的某资源在一段时间内的繁忙程度。
- 成本：开销/性能的比例。

## 3.2 性能分析方法论

> 详见另一本书《性能之巅：洞悉系统、企业与云计算》

- 业务负载画像
- 下钻分析
- USE方法论
    - U：使用率
    - S：饱和度
    - E：错误

## 3.3 Linux 60 秒分析

> https://github.com/iovisor/bcc/blob/master/docs/tutorial.md#0-before-bcc

- `uptime`：快速检查平均负载，3 个数字分别表示 1、5、15 分钟滑动窗口积累值。一个较高的 15 分钟负载与一个较低的 1 分钟负载同时出现，可能意味着已经错过了问题发生现场。
- `dmesg | tail`：显示过去 10 条系统日志，注意寻找可能导致性能问题的错误。
- `vmstat 1`：虚拟内存等统计。检查：
    - r：CPU 上正在执行的和等待执行的进程数量，大于 CPU 数量表示 CPU 资源处于饱和状态。
    - free：空间内存
    - si 和 so：页换入和页换出，如果这些值不是 0，意味着内存紧张。这个值只有在配置开启了交换分区后才会起作用。
    - us、sy、id、wa、st。分别表示用户态时间、系统态时间、空闲、等待 I/O、窃取时间。
        - 窃取时间：stolen time，指虚拟化环境下，被其他客户机所挤占的时间。
- `mpstat -P ALL 1`：打印各个 CPU 各状态时间占用分布。
    - 较高的 %iowait，可以使用磁盘 IO 工具进一步分析；
    - 较高的 %sys，可以使用系统调用跟踪和内核跟踪。
- `pidstat 1`：按进程展示 CPU 的使用情况。
- `iostat -xz 1`：显示存储设备的 IO 指标。
- `free -m`：显示了用 MB 为单位的可用内存。
- `sar -n DEV 1`：查看网络设备/接口指标。
- `sar -n TCP,ETCP 1`：查看 TCP 指标和 TCP 错误信息
    - active/s：每秒本地发起的 TCP 连接数量（通过调用 connect 创建）
    - passive/s：每秒远端发起的 TCP 连接数量（通过调用 accept 创建）
    - retrans/s：每秒 TCP 重传的数量
- `top`：浏览系统和进程的摘要信息。

## 3.4 BCC 工具检查清单

> https://github.com/iovisor/bcc/blob/master/docs/tutorial.md#1-general-performance

- execsnoop：跟踪 execve 系统调用。可用于发现存活时间短的进程。
- opensnoop：跟踪 open 系统调用。可用户发现频繁打开的文件。
- ext4slower：跟踪 ext4 文件系统中的常见操作。可用于发现慢 IO。
    - or btrfs*, xfs*, zfs*
- biolatency：跟踪磁盘 IO 延迟，并以直方图显示。
- biosnoop：打印每次磁盘 IO 请求。
- cachestat：统计文件系统缓存。
- tcpconnect：跟踪每次**主动** TCP 连接。**在输出中应该寻找不寻常的连接请求。**
- tcpaccept：跟踪每次**被动** TCP 连接。
- tcpretrans：跟踪 TCP 重传，打印 源地址、目的地址、内核状态。
    - 如果重传发生在 ESTABLISHED 状态，则进一步寻找外部网络坑存在的问题。
    - 如果重传发生在 SYN_SENT 状态，则坑是 CPU 饱和的一个，也可能是内核丢包引发的。
- runqlat：统计线程等待 CPU 运行的时间。
- profile：CPU 剖析器，周期性对调用栈采样。用于理解哪些代码路径消耗了 CPU 资源。
