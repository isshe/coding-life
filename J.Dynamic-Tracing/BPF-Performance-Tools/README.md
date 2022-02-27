
BPF 之巅
---

本书关注点是 bpftrace 编程，不是 BCC 编程。BBC 编程的复杂度会高很多。

[BPF 性能工具](BPF-performance-tools.png)

开发 BPF 工具的策略：先尝试使用静态跟踪技术（tracepoint、USDT），如果不够再转而使用动态跟踪技术（kprobes、uprobes）

BPF 相关项目

* https://github.com/cilium/ebpf
    * Pure-Go library to read, modify and load eBPF programs and attach them to various hooks in the Linux kernel.
