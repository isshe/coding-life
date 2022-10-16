Probes（探针）
---

四种类型：

* Kernal Probes: These give you dynamic access to internal components in the kernel.
    * Kernal Probes 分为两类：
        * kprobes：Kprobes 允许在执行任何内核指令之前插入 BPF 程序。
        * kretprobes：允许在执行内核指令返回后插入 BPF 程序。

* TracePoints: These provide static access to internal components in the kernel.
    * 在 `/sys/kernel/debug/tracing/events` 可以看到系统中支持的 TracePoints。
* User-Space Probes: These give you dynamic access to programs running in user-space.
    * 也分为：
        * uprobes：执行指令前的钩子
        * uretprobes：执行指令后的钩子
    * 可以用 `mn` 命令来检查程序中是否存在目标指令
* User statically defined tracepoints: These allow static access to programs running in user-space.

这里的“静态”和“非静态”是对于 ABI (application binary interface) 是否稳定来说的，静态就是稳定，在不同的内核版本中，也可通用。


拓展知识
---
* 内核对命令名的长度限制是 16 个字节。
