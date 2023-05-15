
BPF 之巅
---

本书关注点是 bpftrace 编程，不是 BCC 编程。BBC 编程的复杂度会高很多。

[BPF 性能工具](BPF-performance-tools.png)

开发 BPF 工具的策略：先尝试使用静态跟踪技术（tracepoint、USDT），如果不够再转而使用动态跟踪技术（kprobes、uprobes）

BPF 相关项目

* https://github.com/cilium/ebpf
    * Pure-Go library to read, modify and load eBPF programs and attach them to various hooks in the Linux kernel.

重要文档

* [BPF Design Q&A](https://github.com/torvalds/linux/blob/master/Documentation/bpf/bpf_design_QA.rst)
* [Linux Socket Filtering aka Berkeley Packet Filter (BPF)](https://github.com/torvalds/linux/blob/master/Documentation/networking/filter.rst)
* https://github.com/torvalds/linux/tree/master/Documentation/bpf
* bpf(2) man 帮助文档
* bpf-helper(2) man 帮助文档
* "BPF: the universal in-kernal virtual machine", 作者 Jonathan Corbet。
* "BPF internals -II", 作者 Suchakra Sharma。
* Cilium 项目的 "BPF and XDP Refernce Guide"。

相关术语
* PMC：CPU 性能监控计数器。
* NMI：不可掩盖的中断。

