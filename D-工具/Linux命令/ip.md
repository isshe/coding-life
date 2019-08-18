[TOC]

ip
---

显示/操纵路由、设备、策略路由、隧道。

## 概要
```bash
ip [ OPTIONS ] OBJECT { COMMAND | help }

ip [ -force ] -batch filename

OBJECT := { link | address | addrlabel | route | rule | neigh | ntable | tunnel | tuntap | maddress | mroute | mrule | monitor | xfrm | netns | l2tp | tcp_metrics }

OPTIONS := { -V[ersion] | -h[uman-readable] | -s[tatistics] | -r[esolve] | -f[amily] { inet | inet6 | ipx | dnet | link } | -o[neline] | -n[etns] name | -a[ll] | -c[olor] }
```
OBJECT:
* address   - 一个设备的IPv4/IPv6地址。（缩写：a 或 addr）
* addrlabel - 协议地址选择的标签配置。  （缩写：addrl）
* l2tp      - IP隧道以太网(L2TPv3).
* link      - 网络设备。如eth0。       （ 缩写：l）
* maddress  - 多播地址。              （缩写：m 或 maddr）
* monitor   - 监测netlink消息
* mroute    - 组播路由缓存条目。       （缩写：mr）
* mrule     - 组播路由策略数据库中的规则。
* neighbour - 管理 ARP 或 NDISC 缓存条目。（缩写：n or neigh）
* netns     - 管理网络命名空间。
* ntable    - 管理邻居缓存的操作。
* route     - 路由表中的路由规则。       （缩写：r）
* rule      - 路由策略数据库中的规则。   （缩写：ru
* tcp_metrics/tcpmetrics - 管理 TCP Metrics。
* tunnel    - IP隧道。                 （缩写：t）
* tuntap    - 管理 TUN/TAP 设备。
* xfrm      - 管理 IPSec 策略。         （缩写：x）

# 选项
```bash
-V, -Version
    打印ip实用工具/iproute2的版本。

-h, -human, -human-readable
    输出具有人类可读值的后跟后缀的统计信息。

-b, -batch <FILENAME>
    从提供的文件或标准输入中读取命令并调用它们。首次失败将导致ip终止。

-force 
    不在批处理模式出错时终止ip。如果在执行命令期间出现任何错误，则应用程序返回代码将不为零。

-s, -stats, -statistics
    输出更多信息。如果选项出现两次或更多次，则信息量会增加。通常，信息是统计信息或某些时间值。

-d, -details
    输出更多细节信息。

-l, -loops <COUNT>
    指定'ip address flush'逻辑在放弃之前将尝试的最大循环次数。默认值为10。零(0)表示一直循环到删除所有地址。

-f, -family <FAMILY>
    指定要使用的协议族。
    协议族标识符可以是inet，inet6，bridge，ipx，dnet，mpls或link之一。
    如果此选项不存在，则从其他参数中猜出协议族。
    如果命令行的其余部分没有提供足够的信息来猜测，则ip会使用一个默认值，通常是inet或any。 
    link是一个特殊的系列标识符，表示不涉及任何网络协议。

-4     -family inet 的简写.
-6     -family inet6 的简写.
-B     -family bridge 的简写.
-D     -family decnet 的简写.
-I     -family ipx 的简写.
-M     -family mpls 的简写.
-0     -family link 的简写.

-o, -oneline
    将每条记录输出到单一的行，用'\'字符替换换行符。
    当您想要使用wc或grep输出计数记录时，这很方便。

-r, -resolve
    使用系统的名称解析程序来打印DNS名称而不是主机地址。

-n, -netns <NETNS>
    将ip切换到指定的网络命名空间NETNS。 实际上它只是简化执行：
        ip netns exec NETNS ip [ OPTIONS ] OBJECT { COMMAND | help }
    到
        ip -n[etns] NETNS [ OPTIONS ] OBJECT { COMMAND | help }

-a, -all
    对所有对象执行指定的命令，它取决于命令是否支持此选项。

-c, -color
    使用颜色输出。

-t, -timestamp
    使用monitor选项时显示当前时间。
```

# 示例
## 查看详细的接口信息
> ip -c -d -s -s link show
> ip -c -d -s -s link show ens33

## 查看接口地址
> ip addr shwo ens33
> ip -4 addr show ens33
> ip -6 addr show ens33

## 为接口添加地址
> ip addr add <IP 地址/前缀长度> [broadcast <广播地址>] dev <接口名>
> ip addr add 192.168.2.102/24 dev ens33
> IPv6地址加`-6`即可。

## 删除接口地址
> sudo ip addr del 192.168.2.102/24 dev ens33

## 启用接口
> ip link set ens33 up

## 禁用接口
> ip link set ens33 down

## 设置接口MAC地址
设置前需要先禁用接口
> ip link set ens33 address 00:0c:29:a5:ce:35

## 设置接口MTU
> ip link set ens33 mtu 1500

## 添加802.1Q VLAN接口
> ip link add link <接口名> name <子接口名> type vlan id <VLAN ID>
> sudo ip link add link ens33 name ens33.1 type vlan id 10

## 删除一个接口
> sudo ip link del ens33.1

## 查看路由表
> sudo ip route show

## 查看指定目标地址用的那条路由规则
> ip route get 192.168.2.103

## 添加默认路由
> ip route add default via <默认网关> [dev <出接口>]

## 添加路由表项
> ip route add <目标 IP 地址/前缀长度> via <下一跳> [dev <出接口>]
> sudo ip route add 192.168.2.0/24 via 192.168.2.1 dev ens33

## 删除路由表项
> sudo ip route del 192.168.3.0/24 dev ens33

## 查看ARP表
> ip neigh show dev ens33

## 添加永久ARP条目
> ip neigh add <IP 地址> lladdr <以冒号分割的 MAC 地址> dev <接口名> nud permanent
> ip neigh add 192.168.2.149 lladdr e0:d5:5e:a1:d0:d1 dev ens33 nud permanent

## 把动态ARP条目转换为永久ARP条目
> ip neigh change <IP 地址> dev <接口名> nud permanent

## 删除ARP条目
> ip neigh del <IP 地址> dev <接口名>
> ip neigh del 192.168.2.149 dev ens33

## 清空ARP表（不影响永久条目）
> ip neigh flush all

# 参考
* https://zhuanlan.zhihu.com/p/28155886
* https://ss64.com/bash/ip.html
* linux man page
