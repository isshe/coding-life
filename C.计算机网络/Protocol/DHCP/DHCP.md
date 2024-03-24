DHCP
---

DHCP - 动态主机设置协议（Dynamic Host Configuration Protocol）。
DHCP 是一个应用层协议。基于`UDP`。

# 作用
* 用于`内网或网络服务供应商`自动分配 IP 给用户。
* 作为`内部管理员`对所有计算机进行`中央管理`的手段。

# 历史
* DHCP 在 1993 年 10 月成为标准协议，它的前身是`BOOTP`协议。
* BOOTP：BOOTP 是一种网络协议，让电脑或其他周边仪器可以从服务器下载启动程序。

# 数据包类型
* DHCP 发现（DISCOVER）：client 在物理子网上发送广播来寻找可用的服务器。
* DHCP 提供（OFFER）：当 DHCP 服务器收到一个来自客户的 IP 租约请求时，它会提供一个 IP 租约。
* DHCP 请求（REQUEST）：当客户 PC 收到一个 IP 租约提供时，它必须告诉所有其他的 DHCP 服务器它已经接受了一个租约提供。
* DHCP 确认（Acknowledge，ACK）：确认租约，包含租期和客户可能请求的其他所有配置信息。
* DHCP 释放 (RELEASE)：客户端向 DHCP 服务器发送一个请求以释放 DHCP 资源，并注销其 IP 地址。
* DHCP NAK：服务器回复客户，客户要求的网址不能被分配。

# 原理/流程
* 客户主机发送 DHCP 服务器发现 (DISCOVER)**广播包**。
* 服务器收到 DISCOVER 包后，回复`OFFER`**单播或广播包**。【疑问？】
* 客户主机收到 OFFER 包后，发送`REQUEST`**广播包**。
* 服务器回复`ACK`**单播或广播包**。包含分配的 IP 及网关 IP(next server IP)、租约等。

![典型 DHCP 会话的模式](dhcp.png)

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
* Op: 信息类型。如：客户端请求为 1，服务器回复为 2。
* Htype：硬件类型。如：Ethernet(0x01)
* Hlen: 硬件地址长度。如：6。（MAC 地址）
* Hops: 跳数。
* Xid：传输 ID。
* Secs: 过去的时间。【疑问】
* Flags：标记。
* Ciaddr：客户端 IP 地址。（这里应该是续约的时候用的）
* Siaddr：下一个服务器 IP 地址。
* Yiaddr：你的 IP。（分配给客户机的 IP）
* Giaddr：中继代理 IP。【？？？】
* Chaddr：客户机硬件地址。（Ethernet 中就是 Mac 地址）
* Sname：服务器主机名称。
* File：文件名。
* Options：选项。详见[rfc2132](https://tools.ietf.org/html/rfc2132)
    * 1：子网掩码；
    * 3：路由 (网关)；
    * 12：主机名
    * 50：请求的 IP 地址
    * 51：租约时间
    * 53：DHCP 消息类型。即上面的几种类型。
    * 54：服务器标识。

# 疑问
## DHCP 服务器是否可以把一台主机拉黑？
* 这个应该由 DHCP 软件提供。

## 服务器回复为什么是`单播或广播包`？
* Flags 中设置`BROADCAST = 0`时，则是单播；否则是广播。这是为了健壮性，同时兼容单播和多播。

## Secs 字段是哪一个时间段？
* 从获取到 IP 地址或者续约过程开始到现在所消耗的时间。

## Giaddr 字段作用是什么？
* giaddr: 中继代理地址。
* 中继代理相关详见：[博客](http://blog.sina.com.cn/s/blog_712ff0c9010109df.html)

# 相关
* DHCP: RFC 2131
* DHCP6: RFC 3315
* [DHCP wiki](https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol)
* [DHCP wiki 中文](https://zh.wikipedia.org/wiki/动态主机设置协议)
* [rfc2132](https://tools.ietf.org/html/rfc2132)