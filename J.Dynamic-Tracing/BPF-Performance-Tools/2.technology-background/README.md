
第2章 技术背景
---

# 2.1 图释 BPF

BPF 技术之间的联系：

![](bpf-tracing-technologies.png)


# 2.3 拓展版 BPF

BPF 运行时的内部结构：

![](bpf-runtime-internals.png)

## 2.3.6 BPF API

BPF 辅助函数

* 见 include/uapi/linux/bpf.h, 有函数列表及描述、返回。

BPF 系统调用命令

* 见 include/uapi/linux/bpf.h 的 bpf_cmd。

BPF 程序类型

* 见 include/uapi/linux/bpf.h 的 bpf_prog_type。

BPF 映射类型

* 见 include/uapi/linux/bpf.h 的 bpf_map_type。

## 2.3.8 BPF sysfs 接口

在 Linux 4.4 中 BPF 引入了相关命令，可以将 BPF 程序和 BPF 映射通过虚拟文件系统显露出来，位置通常在 `/sys/fs/bpf`，这个能力用术语表示为钉住（pinning）。

使用场景：
* 创建持续运行的 BPF 程序（想 daemon 程序那样）：即使创建程序的进程退出，程序仍可运行。
* 用于用户态和正在运行的 BPF 程序交互：用户态读取、修改映射表。

## 2.3.9 BPF 类型格式（BTF）

BTF（BPF Type Format，BPF 类型格式）是一个元数据的格式，用来将 BPF 程序的源代码信息编码到调试信息中。
选这个名称是因为它描述了数据类型，不过后来被拓展到包含函数信息、源代码、全局变量等信息。

详见：Documentation/bpf/btf.rst。

## 2.3.10 BPF CO-RE

一次编译，到处执行（Compile Once - Run Everywhere，CO-RE）。

比 BCC 更有优势，值得学习。

[BPF 二进制文件：BTF，CO-RE 和 BPF 性能工具的未来](https://www.ebpf.top/post/bpf-co-re-btf-libbpf/)
[BCC 到 libbpf 的转换指南](https://www.ebpf.top/post/bcc-to-libbpf-guid/)

