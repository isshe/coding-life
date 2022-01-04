eBPF Exporter
---

eBPF Exporter 是一个将自定义 BPF 跟踪度量导出到 Prometheus 的工具。

# 安装

```shell
go get -u github.com/cloudflare/ebpf_exporter/...
```

# 导出 BPF 度量

eBPF Exporter 使用 YAML 文件进行配置。
