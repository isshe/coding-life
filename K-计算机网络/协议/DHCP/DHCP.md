---
title: 【网络协议】DHCP-动态主机配置协议
date: 2018-08-18 14:30:31
tags: 
  - 计算机网络
  - DHCP
categories: 网络协议
toc: false
mathjax: false
description:
top: false
---

[TOC]

# DHCP
DHCP - 动态主机设置协议（Dynamic Host Configuration Protocol）。
DHCP是一个应用层协议。基于`UDP`。

# 作用
* 用于`内网或网络服务供应商`自动分配IP给用户。
* 作为`内部管理员`对所有计算机进行`中央管理`的手段。

# 历史
* DHCP在1993年10月成为标准协议，它的前身是`BOOTP`协议。
* BOOTP：BOOTP是一种网络协议，让电脑或其他周边仪器可以从服务器下载启动程序。

# 数据包类型
* DHCP发现（DISCOVER）：client在物理子网上发送广播来寻找可用的服务器。
* DHCP提供（OFFER）：当DHCP服务器收到一个来自客户的IP租约请求时，它会提供一个IP租约。
* DHCP请求（REQUEST）：当客户PC收到一个IP租约提供时，它必须告诉所有其他的DHCP服务器它已经接受了一个租约提供。
* DHCP确认（Acknowledge，ACK）：确认租约，包含租期和客户可能请求的其他所有配置信息。
* DHCP释放(RELEASE)：客户端向DHCP服务器发送一个请求以释放DHCP资源，并注销其IP地址。
* DHCP NAK：服务器回复客户，客户要求的网址不能被分配。

# 原理/流程
* 客户主机发送DHCP服务器发现(DISCOVER)**广播包**。
* 服务器收到DISCOVER包后，回复`OFFER`**单播或广播包**。【疑问？】
* 客户主机收到OFFER包后，发送`REQUEST`**广播包**。
* 服务器回复`ACK`**单播或广播包**。包含分配的IP及网关IP(next server IP)、租约等。

![典型DHCP会话的模式](dhcp.png)

# 协议结构
```
        +-------+--------+--------+--------+
        |8 bits | 8 bits | 8 bits | 8 bits |
        +-------+--------+--------+--------+
        | Op    | Htype  | Hlen   | Hops   |
        +-------+--------+--------+--------+
        | Xid                              |
        +----------------------------------+
        | Secs           | Flags           |
        +----------------+-----------------+
        | Ciaddr                           |
        +----------------------------------+
        | Yiaddr                           |
        +----------------------------------+
        | Siaddr                           |
        +----------------------------------+
        | Giaddr                           |
        +----------------------------------+
        | Chaddr (16 bytes)                |
        +----------------------------------+
        | Sname (64 bytes)                 |
        +----------------------------------+
        | File (128 bytes)                 |
        +----------------------------------+
        | Option (variable)                |
        +----------------------------------+
```
* Op: 信息类型。如：客户端请求为1，服务器回复为2。
* Htype：硬件类型。如：Ethernet(0x01)
* Hlen: 硬件地址长度。如：6。（MAC地址）
* Hops: 跳数。
* Xid：传输ID。
* Secs: 过去的时间。【疑问】
* Flags：标记。
* Ciaddr：客户端IP地址。（这里应该是续约的时候用的）
* Siaddr：下一个服务器IP地址。
* Yiaddr：你的IP。（分配给客户机的IP）
* Giaddr：中继代理IP。【？？？】
* Chaddr：客户机硬件地址。（Ethernet中就是Mac地址）
* Sname：服务器主机名称。
* File：文件名。
* Options：选项。详见[rfc2132](https://tools.ietf.org/html/rfc2132)
    * 1：子网掩码；
    * 3：路由(网关)；
    * 12：主机名
    * 50：请求的IP地址
    * 51：租约时间
    * 53：DHCP消息类型。即上面的几种类型。
    * 54：服务器标识。

# 疑问
## DHCP服务器是否可以把一台主机拉黑？
* 这个应该由DHCP软件提供。

## 服务器回复为什么是`单播或广播包`？
* Flags中设置`BROADCAST = 0`时，则是单播；否则是广播。这是为了健壮性，同时兼容单播和多播。

## Secs字段是哪一个时间段？
* 从获取到IP地址或者续约过程开始到现在所消耗的时间。

## Giaddr字段作用是什么？
* giaddr: 中继代理地址。
* 中继代理相关详见：[博客](http://blog.sina.com.cn/s/blog_712ff0c9010109df.html)

# 相关
* DHCP: RFC 2131
* DHCP6: RFC 3315
* [DHCP wiki](https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol)
* [DHCP wiki中文](https://zh.wikipedia.org/wiki/动态主机设置协议)
* [rfc2132](https://tools.ietf.org/html/rfc2132)