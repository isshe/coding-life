
[TOC]

# DNS
DNS - 域名系统（Domain Name System）。
* 是一个将`域名`和`IP地址`进行映射的`分布式数据库`。
* DNS是应用层协议，使用TCP和UDP的53端口。
* 每一级域名长度的限制是63个字符，域名总长度不能超过253个字符。

# 作用
* 将对人友好的域名转换为对计算机友好的IP地址。

# 原理/过程
域名层级
```
主机名.次级域名.顶级域名.根域名
host.sld.tld.root
www.isshe.xyz.root
abc.isshe.xyz.root
```
分级查询：从根域名开始，依次查询每一级域名的NS记录。
* 从`根域名服务器`查询`顶级域名服务器`的NS记录和A记录；
    * `根域名服务器`是总所周知的，不用查。
* 从`顶级域名服务器`查询`次级域名服务器`的NS记录和A记录；
* 从`次级域名服务器`查询`主机名`的A记录；

分级查询示例：`www.isshe.xyz`
> dig +trace www.isshe.xyz 进行查看

* 列出根域名服务器：
```
.                       227343  IN      NS      e.root-servers.net.
.                       227343  IN      NS      k.root-servers.net.
.                       227343  IN      NS      i.root-servers.net.
.                       227343  IN      NS      d.root-servers.net.
.                       227343  IN      NS      j.root-servers.net.
.                       227343  IN      NS      m.root-servers.net.
.                       227343  IN      NS      b.root-servers.net.
.                       227343  IN      NS      c.root-servers.net.
.                       227343  IN      NS      h.root-servers.net.
.                       227343  IN      NS      f.root-servers.net.
.                       227343  IN      NS      l.root-servers.net.
.                       227343  IN      NS      a.root-servers.net.
.                       227343  IN      NS      g.root-servers.net.
```
* 查询`xyz.`的结果：
```
xyz.                    172800  IN      NS      x.nic.xyz.
xyz.                    172800  IN      NS      y.nic.xyz.
xyz.                    172800  IN      NS      z.nic.xyz.
xyz.                    172800  IN      NS      generationxyz.nic.xyz.
...
;; Received 669 bytes from 192.58.128.30#53(j.root-servers.net) in 651 ms
```
* 查询`isshe.xyz.`的结果：
```
isshe.xyz.              3600    IN      NS      f1g1ns1.dnspod.net.
isshe.xyz.              3600    IN      NS      f1g1ns2.dnspod.net.
...
;; Received 581 bytes from 194.169.218.42#53(x.nic.xyz) in 245 ms
```
* 查询`www.isshe.xyz.`的结果：
```
www.isshe.xyz.          600     IN      CNAME   isshe.coding.me.
isshe.xyz.              86400   IN      NS      f1g1ns1.dnspod.net.
isshe.xyz.              86400   IN      NS      f1g1ns2.dnspod.net.
;; Received 135 bytes from 14.215.155.156#53(f1g1ns1.dnspod.net) in 10 ms
```

# 协议格式

# 记录类型
> 详见[DNS记录类型列表](https://zh.wikipedia.org/wiki/DNS%E8%AE%B0%E5%BD%95%E7%B1%BB%E5%9E%8B%E5%88%97%E8%A1%A8)

常见记录类型：
* A：主机记录。
* CNAME：别名记录。（指向某个A记录）
* AAAA：IPv6主机记录。
* SRV：服务位置记录。

# 疑问

# 相关
* dig命令：可用于DNS相关操作。


