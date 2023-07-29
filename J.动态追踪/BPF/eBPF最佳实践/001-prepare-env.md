# 环境搭建

> Rockylinux 9

# rpm

- 安装 bcc

> 初级：使用 bcc 工具
> https://github.com/iovisor/bcc/blob/master/INSTALL.md

```bash
sudo yum install -y epel-release
sudo yum update -y
sudo yum groupinstall -y "Development tools"
sudo yum install bcc bcc-tools
```

安装完以后，工具应该在 `/usr/share/bcc/tools` 目录下。

- 开发 bpftrace

> 中级：开发 bpftrace 程序
> https://github.com/iovisor/bpftrace/blob/master/INSTALL.md

```bash
sudo yum install -y bpftrace
```

示例：https://github.com/iovisor/bpftrace/blob/master/docs/tutorial_one_liners.md

# deb

- 安装 bcc

> 初级

```bash
sudo apt update
sudo apt-get install bpfcc-tools linux-headers-$(uname -r)
```

- 开发 bpftrace

```bash
sudo apt-get install -y bpftrace
```

## 参考

- https://www.brendangregg.com/blog/2019-01-01/learn-ebpf-tracing.html
