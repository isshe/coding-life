[TOC]

iptables
---

> 此文件只记录 iptables 应用层工具的使用。
> 更多信息详见：
> [wiki](https://wiki.archlinux.org/index.php/Iptables_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87))
> [netfilter/iptables](../../H-Linux内核/5.网络/1.Netfilter/Readme.md)

iptables 是一个配置 Linux 内核防火墙的命令行工具，是 netfilter 项目的一部分。
iptables 可以检测、修改、转发、重定向、丢弃 IPv4 数据包。

# 用法

# 选项
```bash
-j          --jump 指定目标，目标可以是用户定义的链。
-t          --table 指定表，取值有 filter、nat、mangle、raw，默认是 filter。
```

# 示例

- 列出所有规则

    ```bash
    # default table: filter
    iptables -S

    # table: filter/nat/mangle/raw
    iptables -S -t nat
    ```

- 源地址转换

```bash
# 增加
sudo iptables -t nat -A POSTROUTING -p tcp -j MASQUERADE

# 删除
sudo iptables -t nat -D POSTROUTING -p tcp -j MASQUERADE
```

- 目的地址转换

```bash
# 增加
sudo iptables -t nat -A PREROUTING -p tcp -m tcp --dport 20228 -j DNAT --to-destination 10.0.0.233:20228
# 删除
sudo iptables -t nat -D PREROUTING -p tcp -m tcp --dport 20228 -j DNAT --to-destination 10.0.0.233:20228
```

# 疑问



# 拓展

- nftables 是会取代 iptables 成为主要的 Linux 防火墙工具。

# 参考
