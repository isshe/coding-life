
# 动态追踪

## 索引

> TODO：按功能分类
> 系统调用跟踪
> 内存管理跟踪
> 文件系统及存储设备跟踪
> 网络跟踪

## 目标

能回答包括但不限于以下问题：

- 动态追踪技术有哪些？
- 动态追踪技术
- 如何进行性能分析？（各种类型性能指标）
- 什么是动态追踪？是否有静态追踪？有什么区别？
- BCC、bpftrace 是什么？
- BPF 是什么？
- 如何基于 BCC 开发一个 BPF 程序？
- 如何基于 bpftrace 开发一个 BPF 程序？
- 如何基于 C 语言开发一个 BPF 程序？
- 事件类型有哪些？
- 如何生成火焰图？了解火焰图数据格式。

## 命令速查

1. debug 文件路径

```bash
ls /usr/lib/debug/lib64/
ls /usr/lib/debug/.build-id/
```

## 参考文档

- https://blog.openresty.com.cn/cn/dynamic-tracing/
